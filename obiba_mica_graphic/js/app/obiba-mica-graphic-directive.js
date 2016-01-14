/*
 * Copyright (c) 2015 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

'use strict';


mica.ObibaGraphicCharts
  .directive('graphicMainCharts', [function () {
    return {
      restrict: 'EA',
      replace: true,
      scope: {},
      templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular_view_template/graphic-main',
      controller: 'GraphicNetworkMainController'
    };
  }]);