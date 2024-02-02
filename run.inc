<?php
function cfd_hackathon_submissions_run_form($form, &$form_state)
{
	$options_first = _list_of_cfd_hackathon();
	$url_cfd_hackathon_id = (int) arg(2);
	$url_case_study_id = (int) arg(3);
	$cfd_hackathon_data = _cfd_hackathon_information($url_cfd_hackathon_id);
	//$old_case_stduy_data = _cfd_hackathon_old_case_study_data($url_case_study_id);
	if ($cfd_hackathon_data == 'Not found') {
		$url_cfd_hackathon_id = '';
	} //$cfd_hackathon_data == 'Not found'
	if (!$url_cfd_hackathon_id) {
		$selected = isset($form_state['values']['cfd_hackathon']) ? $form_state['values']['cfd_hackathon'] : key($options_first);
	} //!$url_cfd_hackathon_id
	elseif ($url_cfd_hackathon_id == '') {
		$selected = 0;
	} //$url_cfd_hackathon_id == ''
	else {
		$selected = $url_cfd_hackathon_id;
	}
	$form = array();
	$form['cfd_hackathon'] = array(
		'#type' => 'select',
		'#title' => t('Title of the case study'),
		'#options' => _list_of_cfd_hackathon(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'cfd_hackathon_project_details_callback'
		)
	);
	if (!$url_cfd_hackathon_id) {
		$form['cfd_hackathon_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_cfd_hackathon_details"></div>'
		);
		/*$form['selected_cfd_hackathon'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_cfd_hackathon"></div>'
		);*/
	} //!$url_cfd_hackathon_id
	else {
		$cfd_hackathon_default_value = $url_cfd_hackathon_id;
		$cfd_case_study_id = $url_case_study_id;
		$form['cfd_hackathon_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_cfd_hackathon_details">' . _cfd_hackathon_details($cfd_hackathon_default_value, $cfd_case_study_id) . '</div>'
		);
		/*$form['selected_cfd_hackathon'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_cfd_hackathon">' . l('Download Report of the Upgraded Case Study', "case-study-project/download/project-file/" . $cfd_hackathon_default_value) . '<br>' . l('Download Upgraded Case Files', 'case-study-project/full-download/project/' . $cfd_hackathon_default_value) . '<br><br>' . l('Download the report of the Case Study', "https://cfd.fossee.in/case-study-project/download/project-file/" . $cfd_case_study_id) . '<br>' . l('Download Case Files of the Original Case Study', 'https://cfd.fossee.in/case-study-project/full-download/project/' . $cfd_case_study_id) . '</div>'
		);*/
	}
	return $form;
}
function cfd_hackathon_project_details_callback($form, $form_state)
{
	$commands = array();
	$cfd_hackathon_default_value = $form_state['values']['cfd_hackathon'];
	if ($cfd_hackathon_default_value != 0) {
		$form['cfd_hackathon_details']['#markup'] = _cfd_hackathon_details($cfd_hackathon_default_value, $cfd_case_study_id);
		$cfd_hackathon_details = _cfd_hackathon_information($cfd_hackathon_default_value);
		$query = db_select('available_hackathon_project_titles');
    $query->fields('available_hackathon_project_titles');
    $query->condition('project_title', $cfd_hackathon_details->project_title);
    $old_case_study_q = $query->execute();
    $old_case_study_data = $old_case_study_q->fetchObject();
    $cfd_case_study_id = $old_case_study_data->case_study_id;
		$provider = user_load($cfd_hackathon_details->uid);
		if ($cfd_hackathon_details->uid > 0) {
			$commands[] = ajax_command_html('#ajax_selected_cfd_hackathon', l('Download Report of the Upgraded Case Study', "case-study-project/download/project-file/" . $cfd_hackathon_default_value) . '<br>' . l('Download Upgraded Case Files', 'case-study-project/full-download/project/' . $cfd_hackathon_default_value) . '<br><br>' . l('Download the report of the Case Study', "https://cfd.fossee.in/case-study-project/download/project-file/" . $cfd_case_study_id) . '<br>' . l('Download Case Files of the Original Case Study', 'https://cfd.fossee.in/case-study-project/full-download/project/' . $cfd_case_study_id));
		} //$cfd_hackathon_details->uid > 0
		else {
			$commands[] = ajax_command_html('#ajax_selected_cfd_hackathon', '');
			$commands[] = ajax_command_html('#ajax_selected_cfd_hackathon', '');
		}
		$commands[] = ajax_command_html('#ajax_cfd_hackathon_details', _cfd_hackathon_details($cfd_hackathon_default_value, $cfd_case_study_id));
	} //$cfd_hackathon_default_value != 0
	else {
		// $form['lab_experiment_list']['#options'] = _ajax_get_experiment_list();
		//  $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_cfd_hackathon_details', '');
		$commands[] = ajax_command_html('#ajax_selected_cfd_hackathon', '');
		$commands[] = ajax_command_html('#ajax_selected_cfd_hackathon', '');
		$commands[] = ajax_command_data('#ajax_selected_cfd_hackathon', 'form_state_value_select', $form_state['values']['cfd_hackathon']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header) {
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row) {
		$tbody .= "<tr>";
		foreach ($row as $data) {
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}
/*****************************************************/
function _list_of_cfd_hackathon()
{
	$cfd_hackathon_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {cfd_hackathon_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('approval_status', 3);
	$query->condition('is_completed', 1);
	$query->orderBy('uid', 'ASC');
	$cfd_hackathon_titles_q = $query->execute();
	while ($cfd_hackathon_titles_data = $cfd_hackathon_titles_q->fetchObject()) {
		$query_user = db_select('cfd_hackathon_user_profile');
    $query_user->fields('cfd_hackathon_user_profile');
    $query_user->condition('uid', $cfd_hackathon_titles_data->uid);
    //$query_user->condition('approval_status', '1');
    $user_q = $query_user->execute();
    $user_data = $user_q->fetchObject();
		$cfd_hackathon_titles[$cfd_hackathon_titles_data->id] = $cfd_hackathon_titles_data->project_title . ' (Upgraded by ' . $user_data->name_title . ' ' . $user_data->contributor_name . ')';
	} //$cfd_hackathon_titles_data = $cfd_hackathon_titles_q->fetchObject()
	return $cfd_hackathon_titles;
}
function _cfd_hackathon_information($proposal_id)
{
	$query = db_select('cfd_hackathon_proposal', 'c');
	$query->leftJoin('cfd_hackathon_user_profile', 'u', 'c.uid = u.uid');
	$query->fields('c');
	$query->fields('u');
	$query->condition('c.id', $proposal_id);
	$query->condition('c.approval_status', 3);
	$cfd_hackathon_q = $query->execute();
	$cfd_hackathon_data = $cfd_hackathon_q->fetchObject();
	if ($cfd_hackathon_data) {
		return $cfd_hackathon_data;
	} //$cfd_hackathon_data
	else {
		return 'Not found';
	}
}
function _cfd_case_study_data($proposal_id)
{
	$query = db_select('case_study_proposal');
	$query->fields('case_study_proposal');
	$query->condition('id', $proposal_id);
	$query->condition('approval_status', 3);
	$case_study_q = $query->execute();
	$case_study_data = $case_study_q->fetchObject();
	if ($case_study_data) {
		return $case_study_data;
	} //$case_study_data
	else {
		return 'Not found';
	}
	//var_dump($case_study_data);die;
}
function get_version_name($version_id){
	$query = db_select('case_study_software_version');
	$query->fields('case_study_software_version');
	$query->condition('id', $version_id);
	$case_study_q = $query->execute();
	$case_study_data = $case_study_q->fetchObject();
	if ($case_study_data) {
		return $case_study_data->case_study_version;
	} //$case_study_data
	else {
		return 'Not found';
	}
}
function _cfd_hackathon_details($cfd_hackathon_default_value, $cfd_case_study_id)
{
	$cfd_hackathon_details = _cfd_hackathon_information($cfd_hackathon_default_value);
	$case_study_details = _cfd_case_study_data($cfd_case_study_id);
	$case_study_version = get_version_name($case_study_details->version_id);
	//var_dump($case_study_details->faculty_name);die;
	if($case_study_details->faculty_name == NULL || $case_study_details->faculty_name = '' || $case_study_details->faculty_name == '-'){
		$faculty_name = 'NA';
	}
	else{
		$faculty_name = $case_study_details->faculty_name;
	}
	if ($cfd_hackathon_default_value != 0) {
		$form['cfd_hackathon_details']['#markup'] = '<table cellspacing="1" cellpadding="1" border="0" style="width: 100%;" valign="top">' . '<tr><td style="width: 35%;"><span style="color: rgb(128, 0, 0);"><strong>About the Case Study</strong></span></td>
	<td style="width: 35%;"><span style="color: rgb(128, 0, 0);"><strong>Contributor details during the hackathon</strong></span></td></tr>' . '<tr><td valign="top"><ul>' . '<li><strong>Title of the Case Study:</strong> ' . $cfd_hackathon_details->project_title . '</li>' . '<li><strong>Contributor Name:</strong> ' . $case_study_details->name_title . ' ' . $case_study_details->contributor_name . '</li>' . '<li><strong>Version:</strong> ' . $case_study_version . '</li>' . '<li><strong>Faculty Guide:</strong> ' . $faculty_name . '</li>' . '<li><strong>University:</strong> ' . $case_study_details->university . '</li>' . '</ul></td><td valign="top"><ul>' . '<li><strong>Contributor Name:</strong> ' . $cfd_hackathon_details->name_title . ' ' . $cfd_hackathon_details->contributor_name . '</li>' . '<li><strong>Version upgraded to:</strong> ' . $cfd_hackathon_details->version . '<li><strong>Faculty Guide:</strong> ' . $cfd_hackathon_details->faculty_guide_name . '</li>' . '<li><strong>University:</strong> ' . $cfd_hackathon_details->university . '</li>' . '</ul></td></tr>' . '<tr><td style="width: 35%;">' . l('Download the report of the Case Study', "https://cfd.fossee.in/case-study-project/download/project-file/" . $cfd_case_study_id) . '<br>' . l('Download Case Files of the Original Case Study', 'https://cfd.fossee.in/case-study-project/full-download/project/' . $cfd_case_study_id) . '</td>
	<td style="width: 35%;">' . l('Download Report of the Upgraded Case Study', "case-study-project/download/project-file/" . $cfd_hackathon_default_value) . '<br>' . l('Download Upgraded Case Files', 'case-study-project/full-download/project/' . $cfd_hackathon_default_value) . '</td></tr></table>';
		$details = $form['cfd_hackathon_details']['#markup'];
		return $details;
	} //$cfd_hackathon_default_value != 0
}
