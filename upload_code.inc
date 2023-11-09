<?php
function cfd_hackathon_submissions_upload_abstract_code_form($form, &$form_state) {
    $proposal_id = (int) arg(2);
    global $user;
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data",
    );
    /* get current proposal */
    //$proposal_id = (int) arg(3);
    $uid = $user->uid;
    $query = db_select('cfd_hackathon_proposal');
    $query->fields('cfd_hackathon_proposal');
    $query->condition('uid', $uid);
    $query->condition('approval_status', '1');
    $proposal_q = $query->execute();
    if ($proposal_q) {
        if ($proposal_data = $proposal_q->fetchObject()) {
            /* everything ok */
        } //$proposal_data = $proposal_q->fetchObject()
        else {
            drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
            drupal_goto('case-study-project/abstract-code');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-project/abstract-code');
        return;
    }
    $query_user = db_select('cfd_hackathon_user_profile');
    $query_user->fields('cfd_hackathon_user_profile');
    $query_user->condition('uid', $uid);
    //$query_user->condition('approval_status', '1');
    $user_q = $query_user->execute();
    $user_data = $user_q->fetchObject();
    $query = db_select('cfd_hackathon_submitted_abstracts');
    $query->fields('cfd_hackathon_submitted_abstracts');
    $query->condition('proposal_id', $proposal_id);
    $abstracts_q = $query->execute()->fetchObject();
    /*if ($abstracts_q) {
        if ($abstracts_q->is_submitted == 1) {
            drupal_set_message(t('You have already submited your Case Directory, hence you can not upload any more, for any query please write to us.'), 'error', $repeat = false);
            drupal_goto('cfd-hackathon/my-submissions');
            //return;
        } //$abstracts_q->is_submitted == 1
    }*/ //$abstracts_q->is_submitted == 1
    $today = time();
    if($today < $proposal_data->expected_date_of_completion){
    $form['project_title'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->project_title,
        '#title' => t('Title of the Case Study Project'),
    );
    $form['contributor_name'] = array(
        '#type' => 'item',
        '#markup' => $user_data->contributor_name,
        '#title' => t('Contributor Name'),
    );
    $existing_uploaded_S_file = _hackathon_default_value_for_uploaded_files("S", $proposal_data->id);
    if (!$existing_uploaded_S_file) {
        $existing_uploaded_S_file = new stdClass();
        $existing_uploaded_S_file->filename = "No file uploaded";
    } //!$existing_uploaded_S_file
    $form['upload_case_study_developed_process'] = array(
        '#type' => 'file',
        '#title' => t('Upload the Case Directory'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_S_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('hackathon_submissions_project_files_extensions', '') . '</span>',
    );
    $existing_uploaded_A_file = _hackathon_default_value_for_uploaded_files("A", $proposal_data->id);
    if (!$existing_uploaded_A_file) {
        $existing_uploaded_A_file = new stdClass();
        $existing_uploaded_A_file->filename = "No file uploaded";
    } //!$existing_uploaded_S_file
    $form['upload_abstract'] = array(
        '#type' => 'file',
        '#title' => t('Upload the Case Directory'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('hackathon_submissions_abstract_upload_extensions', '') . '</span>',
    );
    $form['prop_id'] = array(
        '#type' => 'hidden',
        '#value' => $proposal_data->id,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
    );
    $form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'case-study-project/abstract-code'),
    );
    return $form;
}
else{
    if($abstracts_q->is_submitted == 1){
        $release_status = 3;
    }
    else{
        $release_status = 0;
    }
    $query = "UPDATE cfd_hackathon_proposal SET approval_status = :release_status WHERE id = :proposal_id";
    $args = array(
        ":release_status" => $release_status,
        ":proposal_id" => $proposal_id
    );
    $result = db_query($query, $args);
    drupal_set_message(t('Project title has been released successfully, you may propose a new title'), 'status');
    drupal_goto('cfd-hackathon/my-submissions');
}
}

///////////////////////////////////////////////////////////////
function cfd_hackathon_submissions_upload_abstract_code_form_validate($form, &$form_state)
{
    //var_dump($form);die;
    if (isset($_FILES['files'])) {
        /* check if atleast one source or result file is uploaded */
        /*if (!($_FILES['files']['name']['upload_case_study_developed_process'])) {
            form_set_error('upload_case_study_developed_process', t('Please upload the project files'));
        }
        if (!($_FILES['files']['name']['upload_abstract'])) {
            form_set_error('upload_abstract', t('Please upload the abstract file'));
        }*/

        /* check for valid filename extensions */
        foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
            if ($file_name) {
                /* checking file type */
                if (strstr($file_form_name, 'upload_case_study_developed_process'))
                        $file_type = 'S';
                    else if (strstr($file_form_name, 'upload_abstract'))
                        $file_type = 'A';
                    $allowed_extensions_str = '';
                    switch ($file_type)
                    {
                        case 'S':
                            $allowed_extensions_str = variable_get('hackathon_submissions_project_files_extensions', '');
                            break;
                        case 'A':
                            $allowed_extensions_str = variable_get('hackathon_submissions_abstract_upload_extensions', '');
                            break;
                    }
                $allowed_extensions = explode(',', $allowed_extensions_str);
                $fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
                $temp_extension = end($fnames);
                if (!in_array($temp_extension, $allowed_extensions)) {
                    form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
                }

                if ($_FILES['files']['size'][$file_form_name] <= 0) {
                    form_set_error($file_form_name, t('File size cannot be zero.'));
                }

                /* check if valid file name */
                if (!cfd_hackathon_submissions_check_valid_filename($_FILES['files']['name'][$file_form_name])) {
                    form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
                }

            } //$file_name
        } //$_FILES['files']['name'] as $file_form_name => $file_name
    }
    return $form_state;
}
function cfd_hackathon_submissions_upload_abstract_code_form_submit($form, &$form_state)
{
    global $user;
    $v = $form_state['values'];
    $root_path = cfd_hackathon_submissions_path();
    $proposal_data = cfd_hackathon_submissions_get_proposal();
    $proposal_id = $proposal_data->id;
    if (!$proposal_data) {
        drupal_goto('');
        return;
    } //!$proposal_data
    $proposal_id = $proposal_data->id;
    $proposal_directory = $proposal_data->directory_name;
    /* create proposal folder if not present */
    //$dest_path = $proposal_directory . '/';
    $dest_path_project_files = $proposal_directory . '/';
    $proposal_id = $proposal_data->id;
    $query_s = "SELECT * FROM {cfd_hackathon_submitted_abstracts} WHERE proposal_id = :proposal_id";
    $args_s = array(
        ":proposal_id" => $proposal_id,
    );
    $query_s_result = db_query($query_s, $args_s)->fetchObject();
    if (!$query_s_result) {
        /* creating solution database entry */
        $query = "INSERT INTO {cfd_hackathon_submitted_abstracts} (
	proposal_id,
	approver_uid,
	abstract_approval_status,
	abstract_upload_date,
	abstract_approval_date,
	is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status,:abstract_upload_date, :abstract_approval_date, :is_submitted)";
        $args = array(
            ":proposal_id" => $proposal_id,
            ":approver_uid" => 0,
            ":abstract_approval_status" => 0,
            ":abstract_upload_date" => time(),
            ":abstract_approval_date" => 0,
            ":is_submitted" => 1,
        );
        $submitted_abstract_id = db_query($query, $args, array(
            'return' => Database::RETURN_INSERT_ID,
        ));
        $query1 = "UPDATE {case_study_proposal} SET is_submitted = :is_submitted WHERE id = :id";
        $args1 = array(
            ":is_submitted" => 1,
            ":id" => $proposal_id,
        );
        db_query($query1, $args1);
        drupal_set_message('Abstract uploaded successfully.', 'status');
    } //!$query_s_result
    else {
        $query = "UPDATE {cfd_hackathon_submitted_abstracts} SET


	abstract_upload_date =:abstract_upload_date,
	is_submitted= :is_submitted
	WHERE proposal_id = :proposal_id
	";
        $args = array(

            ":abstract_upload_date" => time(),
            ":is_submitted" => 1,
            ":proposal_id" => $proposal_id,
        );
        $submitted_abstract_id = db_query($query, $args, array(
            'return' => Database::RETURN_INSERT_ID,
        ));
        $query1 = "UPDATE {case_study_proposal} SET is_submitted = :is_submitted WHERE id = :id";
        $args1 = array(
            ":is_submitted" => 1,
            ":id" => $proposal_id,
        );
        db_query($query1, $args1);
        drupal_set_message('Abstract updated successfully.', 'status');
    }
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
        if ($file_name) {
            /* checking file type */
            if (strstr($file_form_name, 'upload_case_study_developed_process')) {
                $file_type = 'S';
            } //strstr($file_form_name, 'upload_case_study_developed_process')
            if (strstr($file_form_name, 'upload_abstract')) {
                $file_type = 'A';
            } //strstr($file_form_name, 'upload_case_study_developed_process')
            switch ($file_type) {
                case 'S':

                    if (file_exists($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                        //unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
                        drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
                            '!filename' => $_FILES['files']['name'][$file_form_name],
                        )), 'error');
                    } //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    /* uploading file */
                    else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                        /* for uploaded files making an entry in the database */
                        $query_abstracts = "SELECT * FROM cfd_hackathon_submitted_abstracts WHERE proposal_id = :proposal_id";
                        $query_abstracts_args = array(
                            ":proposal_id" => $proposal_id,
                        );
                        $query_abstracts_result = db_query($query_abstracts, $query_abstracts_args)->fetchObject();
                        $submitted_abstract_id = $query_abstracts_result->id;
                        $query_ab_f = "SELECT * FROM cfd_hackathon_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype =
				:filetype";
                        $args_ab_f = array(
                            ":proposal_id" => $proposal_id,
                            ":filetype" => $file_type,
                        );
                        $query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
                        if (!$query_ab_f_result) {
                            $query = "INSERT INTO {cfd_hackathon_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
                            $args = array(
                                ":submitted_abstract_id" => $submitted_abstract_id,
                                ":proposal_id" => $proposal_id,
                                ":uid" => $user->uid,
                                ":approvar_uid" => 0,
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":filetype" => $file_type,
                                ":timestamp" => time(),
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID));
                            drupal_set_message($file_name . ' uploaded successfully.', 'status');
                        } //!$query_ab_f_result
                        else {
                            unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
                            $query = "UPDATE {cfd_hackathon_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
                            $args = array(
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":timestamp" => time(),
                                ":proposal_id" => $proposal_id,
                                ":filetype" => $file_type,
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID,
                            ));

                            drupal_set_message($file_name . ' file updated successfully.', 'status');
                        }
                    } //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    else {
                        drupal_set_message('Error uploading file : ' . $dest_path_project_files . $file_name, 'error');
                    }
                    break;
                    case 'A':

                    if (file_exists($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                        //unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
                        drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
                            '!filename' => $_FILES['files']['name'][$file_form_name],
                        )), 'error');
                    } //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    /* uploading file */
                    else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                        /* for uploaded files making an entry in the database */
                        $query_abstracts = "SELECT * FROM cfd_hackathon_submitted_abstracts WHERE proposal_id = :proposal_id";
                        $query_abstracts_args = array(
                            ":proposal_id" => $proposal_id,
                        );
                        $query_abstracts_result = db_query($query_abstracts, $query_abstracts_args)->fetchObject();
                        $submitted_abstract_id = $query_abstracts_result->id;
                        $query_ab_f = "SELECT * FROM cfd_hackathon_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype =
                :filetype";
                        $args_ab_f = array(
                            ":proposal_id" => $proposal_id,
                            ":filetype" => $file_type,
                        );
                        $query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
                        if (!$query_ab_f_result) {
                            $query = "INSERT INTO {cfd_hackathon_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
                            $args = array(
                                ":submitted_abstract_id" => $submitted_abstract_id,
                                ":proposal_id" => $proposal_id,
                                ":uid" => $user->uid,
                                ":approvar_uid" => 0,
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":filetype" => $file_type,
                                ":timestamp" => time(),
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID));
                            drupal_set_message($file_name . ' uploaded successfully.', 'status');
                        } //!$query_ab_f_result
                        else {
                            unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
                            $query = "UPDATE {cfd_hackathon_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
                            $args = array(
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":timestamp" => time(),
                                ":proposal_id" => $proposal_id,
                                ":filetype" => $file_type,
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID,
                            ));

                            drupal_set_message($file_name . ' file updated successfully.', 'status');
                        }
                    } //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    else {
                        drupal_set_message('Error uploading file : ' . $dest_path_project_files . $file_name, 'error');
                    }
                    break;
            } //$file_type
        } //$file_name
    } //$_FILES['files']['name'] as $file_form_name => $file_name
    /* sending email */
    $email_to = $user->mail;
    $from = variable_get('cfd_hackathon_from_email', '');
    $bcc = variable_get('cfd_hackathon_emails', '');
    $cc = variable_get('cfd_hackathon_cc_emails', '');
    $params['abstract_uploaded']['proposal_id'] = $proposal_id;
    $params['abstract_uploaded']['submitted_abstract_id'] = $submitted_abstract_id;
    $params['abstract_uploaded']['user_id'] = $user->uid;
    $params['abstract_uploaded']['headers'] = array(
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc,
    );
    if (!drupal_mail('cfd_hackathon', 'abstract_uploaded', $email_to, language_default(), $params, $from, true)) {
        drupal_set_message('Error sending email message.', 'error');
    }

    drupal_goto('cfd-hackathon/my-submissions');
}
function _hackathon_default_value_for_uploaded_files($filetype, $proposal_id)
{
    $query = db_select('cfd_hackathon_submitted_abstracts_file');
    $query->fields('cfd_hackathon_submitted_abstracts_file');
    $query->condition('proposal_id', $proposal_id);
    $selected_files_array = "";
    if ($filetype == "S") {
        $query->condition('filetype', $filetype);
        $filetype_q = $query->execute()->fetchObject();
        return $filetype_q;
    } elseif ($filetype == "A") {
        $query->condition('filetype', $filetype);
        $filetype_q = $query->execute()->fetchObject();
        return $filetype_q;
    }
    return;
}