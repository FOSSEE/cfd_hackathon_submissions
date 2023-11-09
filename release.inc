<?php
function cfd_hackathon_release_project_form($form, $form_state){
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
		'#value' => $user->mail,
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
	/*$form['release'] = array(
		'#type' => 'checkbox', 
		'#title' => t('Release the project title'),
	);*/
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Release'),
		'#attributes' => array(
                            'onClick' => 'return confirm("Are you sure you want to release the project title?")'
                        )
	);
	$form['cancel'] = array(
        '#type' => 'markup',
        '#markup' => l(t('Cancel'), 'cfd-hackathon/my-submissions'),
    );
	return $form;
}

function cfd_hackathon_release_project_form_submit($form, $form_state){
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
    drupal_goto('cfd-hackathon/my-submissions');
}