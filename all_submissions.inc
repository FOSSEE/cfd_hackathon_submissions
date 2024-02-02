<?php
function cfd_hackathon_display_all_submissions()
{
    global $user;
    /* get pending proposals to be approved */
    if ($user->uid == 0)
    {
        $msg = drupal_set_message(t('It is mandatory to login on this website to view all submissions. If you are new user please create a new account first.'), 'error');
        //drupal_goto('/pssp');
        drupal_goto('user/login', array('query' => drupal_get_destination()));
        return $msg;
    }
    $output = "";
    $final_submission_rows = array();

    $query = db_select('cfd_hackathon_proposal');
    $query->fields('cfd_hackathon_proposal');
    //$query->condition('approval_status', 2);
    //$query->condition('uid',$user->uid);
    $query->orderBy('id', 'DESC');
    $my_proposals_q = $query->execute();
    while($final_submission_data = $my_proposals_q->fetchObject()){
        $query_user = db_select('cfd_hackathon_user_profile');
    $query_user->fields('cfd_hackathon_user_profile');
    $query_user->condition('uid', $final_submission_data->uid);
    //$query_user->condition('approval_status', '1');
    $user_q = $query_user->execute();
    $user_data = $user_q->fetchObject();
    $today = date("Y-m-d H:i:s");
    if($final_submission_data->approval_status == 1){
    $action =  l('Release', 'cfd-hackathon/release/' . $final_submission_data->id) . ' | ' . l('View Submission', 'cfd-hackathon/view-submission/' . $final_submission_data->id) . ' | ' . l('Mark Completed', 'cfd-hackathon/mark-completed/' . $final_submission_data->id);
}
elseif ($final_submission_data->approval_status == 0) {
    $action = t('This title has been discontinued');
}
elseif ($final_submission_data->approval_status == 3) {
    $action = t('This title has been completed') . ' | ' . l('View Submission', 'cfd-hackathon/view-submission/' . $final_submission_data->id);
}
    $creation_date = date('d-m-Y', $final_submission_data->creation_date);
    $expected_date_of_completion = date('d-m-Y h:i A', $final_submission_data->expected_date_of_completion);
    $final_submission_rows[$final_submission_data->id] = array(
        $creation_date,
        $user_data->contributor_name,
        $final_submission_data->project_title,
        $final_submission_data->version,
        $expected_date_of_completion,
        $action
    );
}
    /* check if there are any pending proposals */
     //!$pending_rows
    $final_submission_header = array(
        'Date of Submission',
        'Name',
        'Project title',
        'Version',
        'Expected Date of Completion',
        ''
    );
    //$output = theme_table($pending_header, $pending_rows);
    $output .= theme('table', array(
        'header' => $final_submission_header,
        'rows' => $final_submission_rows
    ));
    return $output;
}


function cfd_hackathon_submissions_progress_all()
{
    global $user;
    /* get pending proposals to be approved */
    $output = "";
    $final_submission_rows = array();

    $query = db_select('cfd_hackathon_proposal');
    $query->fields('cfd_hackathon_proposal');
    $query->condition('approval_status', 1);
    //$query->condition('uid',$user->uid);
    $query->orderBy('id', 'DESC');
    $my_proposals_q = $query->execute();
    if($my_proposals_q->rowCount() == 0){ 
        $output .= "There are no submissions yet.";
    }
    else{
    while($final_submission_data = $my_proposals_q->fetchObject()){
        $query_user = db_select('cfd_hackathon_user_profile');
    $query_user->fields('cfd_hackathon_user_profile');
    $query_user->condition('uid', $final_submission_data->uid);
    //$query_user->condition('approval_status', '1');
    $user_q = $query_user->execute();
    $user_data = $user_q->fetchObject();
    $today = date("Y-m-d H:i:s");
    $creation_date = date('d-m-Y', $final_submission_data->creation_date);
    $expected_date_of_completion = date('d-m-Y h:i A', $final_submission_data->expected_date_of_completion);
    $final_submission_rows[$final_submission_data->id] = array(
        $creation_date,
        $user_data->contributor_name,
        $final_submission_data->project_title,
        $final_submission_data->version,
        $expected_date_of_completion
    );
}
    /* check if there are any pending proposals */
     //!$pending_rows
    $final_submission_header = array(
        'Date of Submission',
        'Name',
        'Project title',
        'Version',
        'Expected Date of Completion'
    );
    //$output = theme_table($pending_header, $pending_rows);
    $output .= theme('table', array(
        'header' => $final_submission_header,
        'rows' => $final_submission_rows
    ));
    return $output;
}
}

function cfd_hackathon_submissions_completed_proposals()
{
    global $user;
    /* get pending proposals to be approved */
    $output = "";
    $final_submission_rows = array();

    $query = db_select('cfd_hackathon_proposal');
    $query->fields('cfd_hackathon_proposal');
    $query->condition('approval_status', 3);
    $query->condition('is_completed',1);
    $query->orderBy('uid', 'ASC');
    $my_proposals_q = $query->execute();
    $i = $my_proposals_q->rowCount();
    //var_dump($i);die;
    while($final_submission_data = $my_proposals_q->fetchObject()){
        $query_user = db_select('cfd_hackathon_user_profile');
    $query_user->fields('cfd_hackathon_user_profile');
    $query_user->condition('uid', $final_submission_data->uid);
    //$query_user->condition('approval_status', '1');
    $user_q = $query_user->execute();
    $user_data = $user_q->fetchObject();
    $query = db_select('available_hackathon_project_titles');
    $query->fields('available_hackathon_project_titles');
    $query->condition('project_title', $final_submission_data->project_title);
    $old_case_study_q = $query->execute();
    $old_case_study_data = $old_case_study_q->fetchObject();
    $expected_date_of_completion = date('Y', $final_submission_data->expected_date_of_completion);
    $final_submission_rows[$final_submission_data->id] = array(
        //$creation_date,
        $i,
        $user_data->contributor_name,
        l($final_submission_data->project_title, 'cfd-hackathon/case-study-run/' . $final_submission_data->id . '/' . $old_case_study_data->case_study_id),
        $final_submission_data->version,
        $user_data->institute,
        $expected_date_of_completion,
       // $action
    );
    $i--;
}
    /* check if there are any pending proposals */
     //!$pending_rows
    $final_submission_header = array(
        'S. No',
        'Name',
        'Project title',
        'OpenFOAM Version',
        'University/ Institute',
        'Year of Completion'
    );
    //$output = theme_table($pending_header, $pending_rows);
    $output .= theme('table', array(
        'header' => $final_submission_header,
        'rows' => $final_submission_rows
    ));
    return $output;
}