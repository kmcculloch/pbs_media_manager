<?php

/**
 * @file
 * Default theme implementation for pbs show field.
 *
 * Available variables:
 * - $pbs_media_manager_show: The show entity being rendered.
 */
?>
<div class="pbs_media_manager-show-block">
  <?php if ($variables['pbs_media_manager_show']['make_link']) { ?>
  <a href="<?php print $variables['pbs_media_manager_show']['link']?>" ><?php
  }?>
  <?php if ($variables['pbs_media_manager_show']['image_url'] != ''){?>
  <img src="<?php print $variables['pbs_media_manager_show']['image_url']?>" >
  <?php } ?>
  <?php if ($variables['pbs_media_manager_show']['title'] != ''){?>
  <h3 class='pbs_media_manager-show-title'><?php print $variables['pbs_media_manager_show']['title']?></h3>
  <?php } ?>
  <?php if ($variables['pbs_media_manager_show']['make_link']) { ?></a><?php
  } ?>
  <?php if ($variables['pbs_media_manager_show']['description'] != ''){?>
  <p class="pbs_media_manager-show-desc"><?php print $variables['pbs_media_manager_show']['description']?></p>
  <?php } ?>

</div>
