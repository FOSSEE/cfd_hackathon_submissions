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

function cfd_hackathon_submissions_upgraded_report() 
{
	$proposal_id = arg(3);
	//var_dump($proposal_id);die;
	$root_path = cfd_hackathon_submissions_path();
	$query = db_select('cfd_hackathon_submitted_abstracts_file');
	$query->fields('cfd_hackathon_submitted_abstracts_file');
	$query->condition('proposal_id', $proposal_id);
	$query->condition('filetype', 'A');
	$result = $query->execute();
	$cfd_case_study_project_files = $result->fetchObject();
	$query1 = db_select('cfd_hackathon_proposal');
	$query1->fields('cfd_hackathon_proposal');
	$query1->condition('id', $proposal_id);
	$result1 = $query1->execute();
	$case_study = $result1->fetchObject();
	$directory_name = $case_study->directory_name . '/';
	$abstract_file = $cfd_case_study_project_files->filepath;
	ob_clean();
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Cache-Control: public");
	header("Content-Description: File Transfer");
	header("Content-Type: application/pdf");
	header('Content-disposition: attachment; filename="' . $abstract_file . '"');
	header("Content-Length: " . filesize($root_path . $directory_name . $abstract_file));
	header("Content-Transfer-Encoding: binary");
	header("Expires: 0");
	header("Pragma: no-cache");
	readfile($root_path . $directory_name . $abstract_file);
	ob_end_flush();
	ob_clean();
}