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
	$query = db_select('cfd_hackathon_submitted_abstracts');
    $query->fields('cfd_hackathon_submitted_abstracts');
    $query->condition('proposal_id', $proposal_id);
    $abstracts_q = $query->execute()->fetchObject();
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
	if($abstracts_q->is_submitted == 1){
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Resubmit the Abstract'),
	);
}
	$form['back'] = array(
        '#type' => 'item',
        '#markup' => l(t('Go back'), 'cfd-hackathon/all-submissions'),
    );
	return $form;
}

function cfd_hackathon_view_submission_form_submit($form, $form_state){
	global $user;
	$proposal_id = (int) arg(2);
	$v = $form_state['values'];
	$query = "UPDATE cfd_hackathon_submitted_abstracts SET is_submitted = :submission_status WHERE proposal_id = :proposal_id";
	$args = array(
		":submission_status" => 0,
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
	$params['cfd_hackathon_submissions_proposal_resubmit']['proposal_id'] = $proposal_id;
	$params['cfd_hackathon_submissions_proposal_resubmit']['user_id'] = $user_data->uid;
	$params['cfd_hackathon_submissions_proposal_resubmit']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('cfd_hackathon_submissions', 'cfd_hackathon_submissions_proposal_resubmit', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
    drupal_set_message(t('Marked for resubmission and user has been notified'), 'status');
    drupal_goto('cfd-hackathon/all-submissions');
}