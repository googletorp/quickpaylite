<?php

/**
 * @file quickpaylite-cencel-page.tpl.php
 *
 * Theme implementation to display the quickpaylite accept page
 *
 * Available variables:
 * - $transaction: Full transaction object with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_quickpaylite_cancel_page()
 */
?>
<div id="quickpaylite-decline-page-<?php print $transaction->api_module; ?>-<?php print $transaction->api_delta; ?>" class="quickpaylite-decline-page clear-block">
  <p><?php print t('The payment was cancelled.'); ?></p>
</div>
