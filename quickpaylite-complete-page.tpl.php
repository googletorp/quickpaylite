<?php
/**
 * @file quickpaylite-complete-page.tpl.php
 *
 * Theme implementation to display the quickpaylite accept page
 *
 * Available variables:
 * - $transaction: Full transaction object with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_quickpaylite_complete_page()
 */
?>
<div id="quickpaylite-accept-page-<?php print $transaction->api_module; ?>-<?php print $transaction->api_delta; ?>" class="quickpaylite-accept-page clear-block">
  <p><?php print t('Your payment was received. Thank you for doing business with us.'); ?></p>
</div>
