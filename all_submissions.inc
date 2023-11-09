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
    $query->condition('uid',$user->uid);
    $query->orderBy('id', 'DESC');
    $my_proposals_q = $query->execute();
    while($final_submission_data = $my_proposals_q->fetchObject()){
    $today = date("Y-m-d H:i:s");
    if($final_submission_data->approval_status == 1){
    $action =  l('Release', 'cfd-hackathon/release/' . $final_submission_data->id);
}
elseif ($final_submission_data->approval_status == 0) {
    $action = t('This title has been discontinued');
}
elseif ($final_submission_data->approval_status == 3) {
    $action = t('This title has been completed');
}
    $creation_date = date('d-m-Y', $final_submission_data->creation_date);
    $expected_date_of_completion = date('d-m-Y h:i A', $final_submission_data->expected_date_of_completion);
    $final_submission_rows[$final_submission_data->id] = array(
        $creation_date,
       // l($final_submission_data->contributor_name, 'user/' . $final_submission_data->uid),
        $final_submission_data->project_title,
        $expected_date_of_completion,
        $action
    );
}
    /* check if there are any pending proposals */
     //!$pending_rows
    $final_submission_header = array(
        'Date of Submission',
        //'Name',
        'Project title',
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