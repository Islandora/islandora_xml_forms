/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 */
Ext.formbuilder.createPropertiesForm = function() {
    return Ext.create('Ext.form.Panel', {
        title: 'Properties Form',
        region: 'center',
        margin: '1 1 1 0',
        frame: true,
        items:  [{
            xtype: 'fieldset',
            title: 'Root Element',
            items: [{
                xtype: 'textfield',
                id: 'localName',
                name: 'localName',
                fieldLabel: 'Root Element Name',
                allowBlank: false,
                anchor: '100%'
            },{
                xtype: 'textfield',
                id: 'uri',
                name: 'uri',
                fieldLabel: 'Namespace URI',
                anchor: '100%'
            }]
        },{
            xtype: 'fieldset',
            title: 'Schema',
            items: [{
                xtype: 'textfield',
                id: 'schema',
                name: 'schema',
                fieldLabel: 'Name',
                anchor: '100%'
            }]
        },{
            xtype: 'formgrid',
            title: 'Namespaces',
            id: 'namespaces',
            name: 'namespaces',
            height: 300,
            store: this.createMapStore(),
            modelInitTmpl: {
                key: '', 
                value: ''
            },
            columns: [{
                xtype: 'gridcolumn',
                dataIndex: 'key',
                header: 'Prefix',
                sortable: true,
                width: 150,
                field: {
                    type: 'textfield'
                }
            },{
                xtype: 'gridcolumn',
                dataIndex: 'value',
                header: 'URI',
                sortable: true,
                flex: 1,
                field: {
                    type: 'textfield'
                }
            }]
        }],
        listeners: {
            hide: function() {
                Ext.formbuilder.savePropertiesForm();
            }     
        }
    });
};