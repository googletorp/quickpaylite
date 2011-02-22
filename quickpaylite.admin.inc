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

  $form['quickpaylite_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('MD5 secret'),
    '#description' => t('MD5 secret as shown in the Quickpay admin.'),
    '#default_value' => variable_get('quickpaylite_secret', ''),
    '#required' => TRUE,
  );

  $form['quickpaylite_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order id prefix'),
    '#description' => t('Prefix for order ids. Order ids must be uniqe when sent to QuickPay, use this to resolve clashes.'),
    '#default_value' => variable_get('quickpaylite_order_prefix', ''),
    '#element_validate' => array('quickpaylite_order_prefix_validate'),
  );

  /*$options = quickpaylite_all_cards();

  $form['quickpaylite_accepted_cards'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Accepted payment methods'),
    '#description' => t('Which cards or other payment methods to show as accepted in the block. NOTE: Some require special agreements.'),
    '#default_value' => variable_get('quickpaylite_accepted_cards', array_keys(quickpaylite_all_cards())),
    '#options' => $options,
    '#attributes' => array('class' => 'quickpay-cards'),
  );*/

  $form['quickpaylite_autofee'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add transaction fee'),
    '#description' => t('Normally the transaction fee is part of the amount paid, by activating this, the fee is added to the amount charged.'),
    '#default_value' => variable_get('quickpaylite_autofee', FALSE),
  );

  $form['quickpaylite_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t('When active, transactions will be run in test mode, even if the QuickPay account is in production mode. Order ids will get a T appended.'),
    '#default_value' => variable_get('quickpaylite_test_mode', FALSE),
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

function quickpaylite_overview_pager() {
  return '';
}
