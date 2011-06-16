/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 */
Ext.formbuilder.createPreviewPanel =  function(url) { // Use an iframe...
    var view_url = url.replace(/\/edit/i, '/view');
    var preview = "<iframe src='" + view_url + "' width='100%' height='100%'><p>Your browser does not support iframes.</p></iframe>";
    return Ext.create('Ext.form.Panel', {
        title: 'Preview',
        html: preview
    });
};