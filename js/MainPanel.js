/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Create the Application
 */
Ext.formbuilder.createMainPanel = function(children){
    return Ext.create('Ext.panel.Panel', {
        width: 960,
        height: 1725,
        title: 'Form Editor',
        layout: 'border',
        renderTo: 'xml-form-builder-editor',
        items: children,
        defaults: {
            margin: '1 0 1 0',
            frame: true
        },
        tbar: {
            xtype: 'toolbar',
            items: [{
                xtype: 'button',
                text: 'Form Properties',
                handler: function() {
                    Ext.formbuilder.showPropertiesForm();
                }
            },{
                xtype: 'tbfill'
            },{
                xtype: 'button',
                text: 'Save & Preview',
                handler: function() {
                    Ext.formbuilder.showPreview();
                }
            },{
                xtype: 'tbseparator'
            },{
                xtype: 'button',
                text: 'Save',
                handler: function() {
                    var url = window.location.pathname + '/save';
                    var data = {
                        properties: {}, 
                        elements: []
                    };
                    var root = Ext.formbuilder.elementStore.getRootNode().getChildAt(0);
                    data.elements = root.getAssociatedData();
                    Ext.Ajax.request({
                        url: url,
                        params: {
                            data: Ext.encode(data)
                        },
                        success: function(response){
                            alert('Fuck ya!');
                        }
                    });
                }
            }]
        }
    });
};