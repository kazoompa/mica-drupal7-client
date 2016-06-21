<?php
/**
 * @file
 * Obiba Mica Module.
 *
 * Copyright (c) 2016 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
?>

<tr>
  <?php if(!empty($project->request) && obiba_mica_user_has_role('mica-data-access-officer')) : ?>
  <td><?php print $project->request->applicant ?></td>
  <?php endif; ?>
  <td>
    <a href="<?php print MicaClientPathProvider::project($project->id) ?>">
      <?php print obiba_mica_commons_get_localized_field($project, 'title') ?>
    </a>
  </td>
  <td>
    <?php if (!empty($project->request)) : ?>
      <?php if ($project->request->viewable) : ?>
        <a href="<?php print MicaClientPathProvider::data_access_request($project->request->id) ?>"><?php print $project->request->id ?></a>
      <?php else : ?>
        <span><?php print $project->request->id ?></span>
      <?php endif; ?>
    <?php endif ;?>
  </td>
  <td>
    <?php
    if (!empty($content->startDate)) :
      print(convert_and_format_string_date($content->startDate, 'D. d-m-Y'));
    endif;
    ?>
  </td>
  <td>
    <?php
    if (!empty($content->endDate)) :
      print(convert_and_format_string_date($content->endDate, 'D. d-m-Y'));
    endif;
    ?>
  </td>
</tr>
