<?php

/**
 * Handle form settings for Livex api
 * @param $form
 * @param $form_state
 * @return mixed
 */
function messenger_config_form($form, &$form_state)
{

  $form[MESSENGER_URL] = array(
    '#type' => 'textfield',
    '#title' => t('API url'),
    '#default_value' => variable_get(MESSENGER_URL, ''),
    '#description' => t('The api url. eg: http://api.domain.com'),
    '#required' => TRUE,
  );

  $form[MESSENGER_SECRET] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#default_value' => variable_get(MESSENGER_SECRET, ''),
    '#description' => t('API secret.'),
    '#required' => TRUE,
  );

  $form[GIPHY_API] = array(
    '#type' => 'textfield',
    '#title' => t('Giphy api key'),
    '#default_value' => variable_get(GIPHY_API, DEFAULT_GIPHY_API),
    '#description' => t('Enter Giphy api.'),
    '#required' => TRUE,
  );

  $form[GIPHY_EXCLUDE_KEYWORD] = array(
    '#type' => 'textarea',
    '#title' => t('Giphy exclude keywords'),
    '#default_value' => variable_get(GIPHY_EXCLUDE_KEYWORD, ''),
    '#description' => t('Enter a word per line'),
    '#required' => FALSE,
  );


  return system_settings_form($form);


}

/**
 * Empty page callback for messages page
 * @param null $id
 * @return string
 */

function messenger_messages_page($id = NULL)
{

  return '';
}


/**
 * Form validation
 * @param $form
 * @param $form_state
 */
function messenger_config_form_validate($form, &$form_state)
{

  $values = $form_state['values'];
  $url = isset($values[MESSENGER_URL]) ? $values[MESSENGER_URL] : '';
  $secret = isset($values[MESSENGER_SECRET]) ? $values[MESSENGER_SECRET] : '';


  if (!_messenger_url_endsWith($url, '/')) {
    $url .= '/';
  }

  $options = array(
    'method' => 'GET',
    'headers' => array('Content-Type' => 'application/json; charset=UTF-8'),
  );
  $result = drupal_http_request($url . 'api/app/secret/validate?secret=' . $secret, $options);

  if ($result->code != 200) {
    form_set_error(MESSENGER_SECRET, "Invalid secret.");
  }


}

/**
 * Page callback for view user profile by userId
 */

function messenger_find_user_or_create($uid)
{
  $user = user_load($uid);

  $error = new stdClass();
  $error->message = "User not found";
  $error->error = True;

  if (!isset($user->uid)) {
    return drupal_json_output($error);
  }

  $result = messenger_find_or_create_user($user);


  if ($result) {
    $success = new stdClass();
    $success->data = $result;


    return drupal_json_output($success);
  } else {
    $error->message = "An error";
    return drupal_json_output($error);
  }
}
