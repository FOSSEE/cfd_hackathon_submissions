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
        '#type' => 'item',
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
	$email_to = $user_data->mail;
	$form = variable_get('hackathon_submissions_from_email', '');
	$bcc = variable_get('hackathon_submissions_emails', '');
	$cc = variable_get('hackathon_submissions_cc_emails', '');
	$params['cfd_hackathon_submissions_proposal_released']['proposal_id'] = $proposal_id;
	$params['cfd_hackathon_submissions_proposal_released']['user_id'] = $user_data->uid;
	$params['cfd_hackathon_submissions_proposal_released']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('cfd_hackathon_submissions', 'cfd_hackathon_submissions_proposal_released', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
    drupal_set_message(t('Project title has been released successfully, you may propose a new title'), 'status');
    if(in_array("Hackathon Reviewer", $user->roles)){
    	drupal_goto('cfd-hackathon/all-submissions');
    }
    else{
    	drupal_goto('cfd-hackathon/my-submissions');
	}
}

function cfd_hackathon_mark_completed_project_form($form, $form_state){
	$proposal_id = (int) arg(2);
	global $user;
	//var_dump($user->roles);die;
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
	/*$form['mark_completed'] = array(
		'#type' => 'checkbox', 
		'#title' => t('mark_completed the project title'),
	);*/
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Mark Completed'),
		'#attributes' => array(
                            'onClick' => 'return confirm("Are you sure you want to mark the submission as completed?")'
                        )
	);
	$form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'cfd-hackathon/my-submissions'),
    );
	return $form;
}

function cfd_hackathon_mark_completed_project_form_submit($form, $form_state){
	global $user;
	$proposal_id = (int) arg(2);
	$v = $form_state['values'];
	$query = "UPDATE cfd_hackathon_proposal SET approval_status = :mark_completed_status WHERE id = :proposal_id";
	$args = array(
		":mark_completed_status" => 3,
		":proposal_id" => $proposal_id
	);
	$result = db_query($query, $args);
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
	$email_to = $user_data->mail;
	$form = variable_get('hackathon_submissions_from_email', '');
	$bcc = variable_get('hackathon_submissions_emails', '');
	$cc = variable_get('hackathon_submissions_cc_emails', '');
	$params['cfd_hackathon_submissions_proposal_completed']['proposal_id'] = $proposal_id;
	$params['cfd_hackathon_submissions_proposal_completed']['user_id'] = $user_data->uid;
	$params['cfd_hackathon_submissions_proposal_completed']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('cfd_hackathon_submissions', 'cfd_hackathon_submissions_proposal_completed', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
    drupal_set_message(t('Project title has been marked completed successfully, you may propose a new title'), 'status');
    if(in_array("Hackathon Reviewer", $user->roles)){
    	drupal_goto('cfd-hackathon/all-submissions');
    }
    else{
    	drupal_goto('cfd-hackathon/my-submissions');
	}
}