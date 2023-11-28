<?php
function cfd_hackathon_view_submission_form($form, $form_state){
	$proposal_id = (int) arg(2);
	global $user;
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	$query = db_select('cfd_hackathon_user_profile');
	$query->fields('cfd_hackathon_user_profile');
	$query->condition('uid', $proposal_data->uid);
	$user_q = $query->execute();
	$user_data = $user_q->fetchObject();
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	$form['contributor_name'] = array(
		'#type' => 'item',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#markup' => $user_data->name_title . ' ' . $user_data->contributor_name,
		'#maxlength' => 250,
		'#required' => TRUE,
		'#disabled' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Email'),
		'#size' => 30,
		'#markup' => user_load($proposal_data->uid)->mail,
		'#disabled' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'item',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#markup' => $user_data->contact_no,
		'#maxlength' => 250,
		'#disabled' => TRUE
	);
	$form['university'] = array(
		'#type' => 'item',
		'#title' => t('University'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your university.... '
		),
		'#markup' => $user_data->university,
		'#disabled' => TRUE
	);
	$form['institute'] = array(
		'#type' => 'item',
		'#title' => t('Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute.... '
		),
		'#markup' => $user_data->institute,
		'#disabled' => TRUE
	);
	$form['version'] = array(
		'#type' => 'item',
		'#title' => t('Version'),
		'#markup' => $proposal_data->version
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#title' => t('Project title'),
		'#markup' => $proposal_data->project_title
	);
	$form['download_project_files'] = array(
        '#type' => 'item',
        '#markup' => l(t('Download Case files'), 'cfd-hackathon/download/project-files/' . $proposal_data->id),
    );
	/*$form['release'] = array(
		'#type' => 'checkbox', 
		'#title' => t('Release the project title'),
	);*/
	$form['back'] = array(
        '#type' => 'markup',
        '#markup' => l(t('Go back'), 'cfd-hackathon/all-submissions'),
    );
	return $form;
}

/*function cfd_hackathon_view_submission_form_submit($form, $form_state){
	global $user;
	$proposal_id = (int) arg(2);
	$v = $form_state['values'];
	$query = "UPDATE cfd_hackathon_proposal SET approval_status = :release_status WHERE id = :proposal_id";
	$args = array(
		":release_status" => 0,
		":proposal_id" => $proposal_id
	);
	$result = db_query($query, $args);
    drupal_set_message(t('Project title has been released successfully, you may propose a new title'), 'status');
    drupal_goto('cfd-hackathon/all-submissions');
}*/