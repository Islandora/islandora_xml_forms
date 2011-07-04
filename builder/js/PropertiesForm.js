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
        buttons: [{
            text: 'Save',
            handler: function() {
                var form = this.up('form').getForm();
                if (form.isValid()) {
                    var record = form.getRecord();
                    // Start
                    record.beginEdit();
                    // Normal form fields
                    var values = form.getValues();
                    for(var i in values) {
                        record.set(i, values[i]);
                    }
                    // Grids
                    var toObject = function(store) {
                        var output = {};
                        store.each(function(item){
                            item = item.data;
                            output[item.key] = item.value;
                        });
                        return output;
                    }
                    var store = Ext.getCmp('namespaces').store;
                    record.set('namespaces', toObject(store));
                    // End
                    record.endEdit();
                }
                
            }
        }],
        items:  [{
            xtype: 'fieldset',
            title: 'Root Element',
            items: [{
                xtype: 'textfield',
                id: 'name',
                name: 'name',
                fieldLabel: 'Local Name',
                allowBlank: false,
                anchor: '100%'
            },{
                xtype: 'textfield',
                name: 'prefix',
                fieldLabel: 'Namespace Prefix',
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
        }]
    });
};