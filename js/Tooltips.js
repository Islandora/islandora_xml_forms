/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Ext.formbuilder.createToolTips = function() {
    var tooltips = [{
        tile: 'After Build',
        target: 'after_build',
        anchor: 'top',
        anchorOffset: 85, // center the anchor on the tooltip
        html: '<h3><a name="after_build" id="after_build"></a>#after_build</h3>'+
    '<p><strong>Used by</strong>: All elements and forms</p>' +
    '<p><strong>Description</strong>: An array of function names which will be called after the form or element is built.</p>'
    }];
    Ext.each(tooltips, function(config) {
        Ext.create('Ext.tip.ToolTip', config);
    });  
    Ext.QuickTips.init();
};