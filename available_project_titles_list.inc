<?php
// case study display completed proposals
function list_of_available_project_titles_form($form, $form_state)
{
	$options_first = array(
		'OpenFOAM v10' => 'OpenFOAM v10',
		'OpenFOAM v2212' => 'OpenFOAM v2212'
	);
	$selected = isset($form_state['values']['version']) ? $form_state['values']['version'] : key($options_first);
	$form = array();
	$form['version'] = array(
		'#type' => 'select',
		'#title' => t('Select the version'),
		'#options' => $options_first,
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'project_titles_list_callback'
		)
	);
	//var_dump(_project_titles($selected));die;
	$form['case_study_titles'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax-case-study-details">' . _project_titles($selected) . '</div>'
		);
	//var_dump($form);die;
	return $form;
}

function project_titles_list_callback($form, $form_state)
{
	$commands = array();
	$case_study_default_value = $form_state['values']['version'];
		$form['case_study_titles']['#markup'] = _project_titles($case_study_default_value);
		$commands[] = ajax_command_html('#ajax-case-study-details', _project_titles($case_study_default_value));
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}

function _project_titles($version){
	$preference_rows = array();
	$query = db_query("SELECT * from available_hackathon_project_titles WHERE project_title NOT IN( SELECT  project_title from cfd_hackathon_proposal WHERE (approval_status = 1 OR approval_status = 3) and version =:version)", array(":version"=>$version));
	$i = $query->rowCount();
	while($result = $query->fetchObject()) {
		$preference_rows[] = array(
				$i,
				//print_r(array_keys($case_studies_list))
				l($result->project_title, 'case-study-project/download/project-title-file/' .$result->case_study_id),
				$result->complexity,
				$result->no_of_days
				);
			$i--;
	}
		$preference_header = array(
			'No',
			'List of available projects',
			'Complexity',
			'No of days to complete'
		);
		$form['case_study_titles']['#markup'] = theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	$output = $form['case_study_titles']['#markup'];
	return $output;
}