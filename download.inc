<?php
// $Id$
function cfd_hackathon_submissions_project_files()
{
	global $user;
	$id = arg(3);
	$root_path = cfd_hackathon_submissions_path();
	//var_dump($root_path);die;
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('id', $id);
	$cfd_hackathon_q = $query->execute();
	$cfd_hackathon_data = $cfd_hackathon_q->fetchObject();
	$cfd_hackathon_PATH = $cfd_hackathon_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('id', $id);
	$circuit_simulation_udc_q = $query->execute();
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('id', $id);
	$query = db_select('cfd_hackathon_submitted_abstracts_file');
	$query->fields('cfd_hackathon_submitted_abstracts_file');
	$query->condition('proposal_id', $id);
	$project_files = $query->execute();
	while ($cfd_project_files = $project_files->fetchObject()) {
		$zip->addFile($root_path . $cfd_hackathon_PATH . $cfd_project_files->filepath, $cfd_hackathon_PATH . str_replace(' ', '_', basename($cfd_project_files->filename)));
	}
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $cfd_hackathon_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $cfd_hackathon_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are no case files to download", 'error');
		drupal_goto('cfd-hackathon/all-submissions');
	}
}