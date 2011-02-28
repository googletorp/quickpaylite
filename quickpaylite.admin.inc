<?php
// $Id: scaffolding_example.admin.inc,v 1.2 2008/06/02 15:46:54 weitzman Exp $

/**
 * @file
 * Administrative pages for the module.
 *
 * Contains form building functions, submit handlers, and theme functions for
 * the module's overview form, add and edit forms, and the delete confirmation
 * form.
 */

/**
 * Gererel settings form.
 */
function quickpaylite_settings_form() {
  drupal_add_css(drupal_get_path('module', 'quickpay') . '/quickpay.css');

  $form['quickpaylite_merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant number'),
    '#description' => t('Merchant id as shown in the QuickPay admin. NOTE: <em>not</em> PBS id'),
    '#default_value' => variable_get('quickpaylite_merchant', ''),
    '#required' => TRUE,
  );

  $form['quickpaylite_md5secret'] = array(
    '#type' => 'textfield',
    '#title' => t('MD5 secret'),
    '#description' => t('MD5 secret as shown in the Quickpay admin.'),
    '#default_value' => variable_get('quickpaylite_md5secret', ''),
    '#required' => TRUE,
  );

  $form['quickpaylite_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order id prefix'),
    '#description' => t('Prefix for order ids. Order ids must be uniqe when sent to QuickPay, use this to resolve clashes.'),
    '#default_value' => variable_get('quickpaylite_order_prefix', ''),
    '#element_validate' => array('quickpaylite_order_prefix_validate'),
  );

  $form['quickpaylite_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#description' => t('Select which currency should be used for transactions.'),
    '#default_value' => variable_get('quickpaylite_currency', ''),
    '#options' => quickpaylite_currencies(),
  );

  $form['quickpaylite_autofee'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add transaction fee'),
    '#description' => t('Normally the transaction fee is part of the amount paid, by activating this, the fee is added to the amount charged.'),
    '#default_value' => variable_get('quickpaylite_autofee', FALSE),
  );

  $form['quickpaylite_autocapture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instant capture'),
    '#description' => t('Capture the payment instantly, this cannot be used if you are selling physical products.'),
    '#default_value' => variable_get('quickpaylite_autocapture', FALSE),
  );

  $form['quickpaylite_testmode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t('When active, transactions will be run in test mode, even if the QuickPay account is in production mode. Order ids will get a T appended.'),
    '#default_value' => variable_get('quickpaylite_testmode', FALSE),
  );

  $languages = array(
    'da' => t('Danish'),
    'de' => t('German'),
    'en' => t('English'),
    'fo' => t('Faeroese'),
    'fr' => t('French'),
    'gl' => t('Greenlandish'),
    'it' => t('Italian'),
    'no' => t('Norwegian'),
    'nl' => t('Dutch'),
    'pl' => t('Polish'),
    'se' => t('Swedish'),
  );

  // FIXME: work together with i18n
  $form['quickpaylite_hosted_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('The language for the credit card form.'),
    '#options' => $languages,
    '#default_value' => variable_get('quickpaylite_hosted_language', 'en'),
  );

  return system_settings_form($form);
}

/**
 * List of implementations of the quickpaylite module.
 * Link to edit settings for each implementation
 * @return string
 */
function quickpaylite_admin_list_implementations(){
  $header = array(t('Module'), t('Implementation'), '');
  $rows = array();
  foreach ((array)module_implements('quickpayliteapi') as $module) {
    foreach ((array)module_invoke($module, 'quickpayliteapi') as $delta => $name) {
      $module_info = unserialize(db_result(db_query("SELECT info FROM {system} WHERE name = '%s'", $module)));
      $rows[] = array($module_info['name'], $name['info'], l(t('Edit'), "admin/settings/quickpaylite/edit/$module/$delta"));
    }
  }
  return theme('table', $header, $rows);
}

/**
 * The general settings form, for quickpaylite.
 */
function quickpaylite_admin_settings_form($form_state, $module, $delta) {
  $module_info = unserialize(db_result(db_query("SELECT info FROM {system} WHERE name = '%s'", $module)));
  $human_delta = module_invoke($module, 'quickpayliteapi', 'info', $delta);
  drupal_set_title(t('Change quickpaylite implementation settings for %name', array('%name' => t($module_info['name']) . ': ' . $human_delta['info'])));

  $implementation = db_fetch_object(db_query("SELECT * FROM {quickpaylite_implementations} WHERE api_module = '%s' AND api_delta = '%s' LIMIT 1;", $module, $delta));
  if (empty($implementation)) {
    db_query("INSERT INTO {quickpaylite_implementations} (api_module, api_delta) VALUES ('%s', '%s')", $module, $delta);
    $implementation = new stdClass();
  }

  $form = array();

  $form['merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant number'),
    '#description' => t('your unique merchantnumber from quickpaylite.'),
    '#default_value' => $implementation->merchant,
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['md5secret'] = array(
    '#type' => 'textfield',
    '#title' => t('MD5 secret'),
    '#description' => t('MD5 secret as shown in the Quickpay admin.'),
    '#default_value' => $implementation->md5secret,
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['autofee'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cardholder pays for the transaction fee.'),
    '#description' => t('If the cardholder should pay for the transaction fees'),
    '#default_value' => $implementation->autofee,
  );

  $form['autocapture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instant capture'),
    '#description' => t('Capture the payment instantly, this cannot be used if you are selling physical products.'),
    '#default_value' => $implementation->instantcapture,
  );

  $form['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency type'),
    '#description' => t('The currency used for payments.'),
    '#options' => quickpaylite_currencies(),
    '#default_value' => $implementation->currency ? $implementation->currency : 'DKK',
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Quickpaylite window language type'),
    '#description' => t('The language of the payment window.'),
    '#options' => array(
      'da' => t('Danish'),
      'de' => t('German'),
      'en' => t('English'),
      'fo' => t('Faeroese'),
      'fr' => t('French'),
      'gl' => t('Greenlandish'),
      'it' => t('Italian'),
      'no' => t('Norwegian'),
      'nl' => t('Dutch'),
      'pl' => t('Polish'),
      'se' => t('Swedish'),
    ),
    '#default_value' => $implementation->language,
  );

  $form['api_module'] = array(
    '#type' => 'value',
    '#value' => $module,
  );

  $form['api_delta'] = array(
    '#type' => 'value',
    '#value' => $delta,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for quickpaylite_admin_settings_form
 */
function quickpaylite_admin_settings_form_submit($form, &$form_state) {
  drupal_write_record('quickpaylite_implementations', $form_state['values'], array('api_delta', 'api_module'));
}

/**
 * Menu callback, displaying a sortable table with pager, displaying the transactions.
 */
function quickpaylite_admin_transactions() {
  $header = array(
    array('data' => t('Order id'), 'field' => 'order_id'),
    array('data' => t('Amount'), 'field' => 'amount'),
    array('data' => t('Currency'), 'field' => 'currency'),
    t('Username'),
    t('Order text'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Logs'), 'field' => 'log_count'),
    array('data' => t('Transaction time'), 'field' => 'payment_time', 'sort' => 'desc'),
  );
  $sql = "SELECT t.*, COUNT(l.log_id) log_count FROM {quickpaylite_transactions} t
          LEFT JOIN {quickpaylite_transaction_logs} l ON t.transaction_id = l.transaction_id
          GROUP BY t.transaction_id";
  $sql .= tablesort_sql($header);
  $limit = 25;
  $result = pager_query($sql, $limit);
  $rows = array();
  $currency_codes = quickpaylite_currencies();
  $status_codes = _quickpaylite_transaction_status();
  while ($transaction = db_fetch_object($result)) {
    $username = t('Anonymous');
    if ($transaction->customer_uid > 0) {
      $account = user_load($transaction->customer_uid);
      if (!empty($account)) {
        $username = check_plain($account->name);
      }
    }
    $rows[] = array(
      $transaction->order_id,
      theme('quickpaylite_number', $transaction->amount/100),
      $currency_codes[$transaction->currency],
      $username,
      truncate_utf8(check_plain($transaction->ordre_text), 22, FALSE, TRUE),
      $status_codes[$transaction->status],
      user_access('quickpaylite_admin_transaction_logs') ? l($transaction->log_count, "admin/settings/quickpaylite/transaction/$transaction->transaction_id/logs") : $transaction->log_count,
      format_date($transaction->payment_time, 'short'),
    );
  }
  return theme('table', $header, $rows) . theme('pager', NULL, $limit);
}

/**
 * Menu callback, displaying a table, displaying the logs for a transaction.
 */
function quickpaylite_admin_transaction_logs($transaction) {
  $header = array(
    t('Status'),
    t('Message'),
    t('Time'),
  );
  $rows = array();
  $result = db_query("SELECT * FROM {quickpaylite_transaction_logs} WHERE transaction_id = %d ORDER BY time", $transaction->transaction_id);
  while ($log = db_fetch_object($result)) {
    $rows[] = array(
      $log->status,
      $log->log_message,
      format_date($log->time, 'short'),
    );
  }
  return l(t('Return to transaction overview'), 'admin/settings/quickpaylite/transactions') . theme('table', $header, $rows);
}
