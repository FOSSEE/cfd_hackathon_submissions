<?php
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Completed
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes
is_completed
0 - in progress
1 - Completed
Tables :
case_study_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
function cfd_hackathon_submissions_proposal_form($form, &$form_state, $no_js_use = FALSE)
{
	global $user;
	/************************ start approve book details ************************/
	if ($user->uid == 0)
	{
		$msg = drupal_set_message(t('It is mandatory to ' . l('login', 'user') . ' on this website to access the case study proposal form. If you are new user please create a new account first.'), 'error');
		drupal_goto('user/login', array('query' => drupal_get_destination()));
		return $msg;
	} //$user->uid == 0
	$query = db_select('cfd_hackathon_proposal');
	$query->fields('cfd_hackathon_proposal');
	$query->condition('uid', $user->uid);
	$query->orderBy('id', 'DESC');
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	$query = db_select('cfd_hackathon_user_profile');
	$query->fields('cfd_hackathon_user_profile');
	$query->condition('uid', $user->uid);
	$user_q = $query->execute();
	$user_data = $user_q->fetchObject();
	//var_dump($user_data);die;
	if ($proposal_data)
	{
		if ($proposal_data->approval_status == 1)
		{
			drupal_set_message(t('We have already received your proposal.'), 'status');
			drupal_goto('');
			return;
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	if(!$user_data){
	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof',
			'Mr' => 'Mr',
			'Ms' => 'Ms'
		),
		'#required' => TRUE
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#maxlength' => 250,
		'#required' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#maxlength' => 250
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your university.... '
		)
	);
	$form['institute'] = array(
		'#type' => 'textfield',
		'#title' => t('Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute.... '
		)
	);
	
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your country name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _cfd_hackathon_list_of_states(),
		'#validated' => TRUE,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City'),
		'#options' => _cfd_hackathon_list_of_cities(),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 6
	);
}
else{
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#default_value' => $user_data->name_title . ' ' . $user_data->contributor_name,
		'#maxlength' => 250,
		'#required' => TRUE,
		'#disabled' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#default_value' => $user_data->contact_no,
		'#maxlength' => 250,
		'#disabled' => TRUE
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your university.... '
		),
		'#default_value' => $user_data->university,
		'#disabled' => TRUE
	);
	$form['institute'] = array(
		'#type' => 'textfield',
		'#title' => t('Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute.... '
		),
		'#default_value' => $user_data->institute,
		'#disabled' => TRUE
	);
}
	/***************************************************************************/
	$form['hr'] = array(
		'#type' => 'item',
		'#markup' => '<hr>'
	);
	$version_options = _cfd_hackathon_list_of_versions();
	$form['version'] = array(
		'#type' => 'select',
		'#title' => t('Version used'),
		'#options' => array(
			'OpenFOAM v10' => 'OpenFOAM v10',
			'OpenFOAM v2212' => 'OpenFOAM v2212'
		),
		'#ajax' => array(
            'event'=>'change',
            'callback' =>'project_title_callback',
            'wrapper' => 'dropdown-project-titles',
        ),
		'#required' => TRUE
	);
	$form['date_of_proposal'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date of Proposal'),
		'#default_value' => date("Y-m-d H:i:s"),
		'#date_format' => 'd-m-Y',
		'#disabled' => TRUE,
		'#date_label_position' => '',
	);
	$list_case_study = _cfd_hackathon_list_of_case_studies();
	#var_dump($list_case_study);die;
	$selected_version = isset($form_state['values']['version'])?$form_state['values']['version']:key($list_case_study);

    $form['project_title'] = array(
        '#title' => t('Project title'),
        '#type' => 'select',
         '#prefix' => '<div id="dropdown-project-titles">',
        '#suffix' => '</div>',
        '#options' => _cfd_hackathon_list_of_case_studies($selected_version),
        '#ajax' => array(
            //'event'=>'replace',
            'callback' =>'expected_date_of_completion_callback',
            //'wrapper' => 'expected-completion-date',
        ),
        '#required' => TRUE,
        '#states' => array(
			'invisible' => array(
				':input[name="version"]' => array(
					'value' => ''
				)
			)
		)
    ); 
	if (isset($form_state['values']['expected_date_of_completion'])) {
        $expected_completion_date = $form_state['values']['expected_date_of_completion'];
      }
      else {
        $expected_completion_date = "";
      }
	//var_dump($form_state['values']['project_title']);die;
	$form['expected_date_of_completion'] = array(
		'#type' => 'textfield',
		'#title' => t('Expected Date of Completion'),
		'#size' =>20,
		'#value' => $expected_completion_date,
		'#maxlength' => 20,
		'#prefix' => '<div id="expected-completion-date">',
		'#suffix' => '</div>',
		'#required' => TRUE,
		'#disabled' => TRUE,
		/*'#states' => array(
			'invisible' => array(
				':input[name="version"]' => array(
					'value' => ''
				)
			)
		)*/
	);
	/*$form['term_condition'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('<a href="/cfd-hackathon/term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
		),
		'#required' => TRUE
	);*/
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function project_title_callback($form, $form_state){
    return $form['project_title'];
}
function expected_date_of_completion_callback($form, $form_state)
{
	$commands = array();
	$case_study_project_default_value = trim($form_state['values']['project_title']);
	$creation_date = $form_state['values']['date_of_proposal'];
	//var_dump($creation_date);die;
	if ($case_study_project_default_value != '')
	{
	//	var_dump($case_study_project_default_value . "    " . $creation_date);die;
		$form_state['values']['expected_date_of_completion'] = _expected_date_of_completion($case_study_project_default_value, $creation_date);
		$form['expected_date_of_completion']['#value'] = _expected_date_of_completion($case_study_project_default_value, $creation_date);
		//var_dump($form_state['values']['expected_date_of_completion']);die;
		$commands[] = ajax_command_replace('#expected-completion-date', drupal_render($form['expected_date_of_completion']));
		//$commands[] = ajax_command_html('#expected-completion-date', '');
		//var_dump($form_state['values']['expected_date_of_completion']);die;
	} //$case_study_project_default_value != 0
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}

function cfd_hackathon_submissions_proposal_form_submit($form, &$form_state)
{
	global $user;
	$root_path = cfd_hackathon_submissions_path();
	if (!$user->uid)
	{
		drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
		return;
	}
	/* inserting the user proposal */
	$v = $form_state["values"];

	//var_dump($v['expected_date_of_completion']);die;
	$project_title = trim($v['project_title']);
	$creation_date = $v['date_of_proposal'];
	$expected_completion_date = strtotime(_expected_date_of_completion($project_title, $creation_date));
	//$expected_completion_date = strtotime("+1day", strtotime($completion_date));
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$directory_name = _cfd_hackathon_dir_name($project_title, $proposar_name);
	$query = db_select('cfd_hackathon_user_profile');
	$query->fields('cfd_hackathon_user_profile');
	$query->condition('uid', $user->uid);
	$user_q = $query->execute();
	$user_data = $user_q->fetchObject();
	if(!$user_data){
	$user_profile_query = "INSERT INTO cfd_hackathon_user_profile 
    (
    uid, 
    name_title, 
    contributor_name,
    contact_no,
    university,
    institute,
    city, 
    pincode, 
    state, 
    country
    ) VALUES
    (
    :uid, 
    :name_title, 
    :contributor_name, 
    :contact_no,
    :university, 
    :institute,
    :city, 
    :pincode, 
    :state,  
    :country
    )";
	$args = array(
		":uid" => $user->uid,
		":name_title" => $v['name_title'],
		":contributor_name" => trim($v['contributor_name']),
		":contact_no" => $v['contributor_contact_no'],
		":university" => $v['university'],
		":institute" => _cfd_hackathon_sentence_case($v['institute']),
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => $v['country']
		);
	$result = db_query($user_profile_query, $args);
	}
	$proposal_query = "INSERT INTO cfd_hackathon_proposal 
    (
    uid, 
    approver_uid,
    project_title, 
    version,
    directory_name,
    approval_status,
    is_completed, 
    dissapproval_reason,
    creation_date, 
    expected_date_of_completion,
    approval_date
    ) VALUES
    (
    :uid, 
    :approver_uid, 
    :project_title, 
    :version_id,
    :directory_name,
    :approval_status,
    :is_completed, 
    :dissapproval_reason,
    :creation_date, 
    :expected_date_of_completion,
    :approval_date
    )";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":project_title" => $project_title,
		":version_id" => $v['version'],
		":directory_name" => $directory_name,
		":approval_status" => 1,
		":is_completed" => 0,
		":dissapproval_reason" => "NULL",
		":creation_date" => time(),
		":expected_date_of_completion" => $expected_completion_date,
		":approval_date" => 0
		);
	$proposal_id = db_query($proposal_query, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	//var_dump($result1->id);die;
	$query_pro = db_select('case_study_proposal');
	$query_pro->fields('case_study_proposal');
//	$query_pro->condition('id', $proposal_data->id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
//	$proposal_id = $abstracts_pro->id;
	$dest_path = $directory_name . '/';
	$dest_path1 = $root_path . $dest_path;
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	if (!$proposal_id)
	{
		drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
		return;
	} //!$proposal_id
	/* sending email */
	/*$email_to = $user->mail;
	$form = variable_get('case_study_from_email', '');
	$bcc = variable_get('case_study_emails', '');
	$cc = variable_get('case_study_cc_emails', '');
	$params['case_study_proposal_received']['result1'] = $result1;
	$params['case_study_proposal_received']['user_id'] = $user->uid;
	$params['case_study_proposal_received']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('case_study', 'case_study_proposal_received', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');*/
	drupal_set_message(t('We have received your proposal.'), 'status');
	drupal_goto('cfd-hackathon/my-submissions');
}