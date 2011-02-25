<?php

/**
 * @file quickpaylite.theme.inc
 * Theme implementations for quickpaylite module.
 */

/**
 * Preprocess function for quickpaylite_cancel_page
 *
 * @param array $variables
 */
function template_preprocess_quickpaylite_cancel_page(&$vars) {
  $transaction = $vars['transaction'];
  
  $vars['template_files'][] = 'quickpaylite-cancel-page-' . $transaction->api_module;
  $vars['template_files'][] = 'quickpaylite-cancel-page-' . $transaction->api_module . '-' . $transaction->api_delta;
}

/**
 * Preprocess function for quickpaylite_complete_page
 *
 * @param array $variables
 */
function template_preprocess_quickpaylite_complete_page(&$vars) {
  $transaction = $vars['transaction'];
  $vars['template_files'][] = 'quickpaylite-complete-page-' . $transaction->api_module;
  $vars['template_files'][] = 'quickpaylite-complete-page-' . $transaction->api_module . '-' . $transaction->api_delta;
}
