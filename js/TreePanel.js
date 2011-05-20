/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Ext.formbuilder.createTreePanel = function() {
    return Ext.create('Ext.tree.Panel', {
        viewConfig: {
            plugins: {
                ptype: 'treeviewdragdrop'
            }
        },
        title: 'Elements',
        store: this.elementStore,
        region: 'west',
        width: 230,
        margin: '1 0 1 1',
        autoScroll: true,
        rootVisible: false,
        split: true,
        tbar: {
            xtype: 'toolbar',
            items: [{
                xtype: 'button',
                text: 'Add',
                handler: function() {
                    var tree = Ext.formbuilder.treePanel;
                    var selectionModel = tree.getSelectionModel();
                    var selection = selectionModel.getSelection();
                    if(selection.length > 0) {
                        var element = new Element({
                            text: 'new element',
                            children: []
                        });
                        var selected = selection[0];
                        var node = selected.createNode(element);
                        selected.appendChild(node);
                        selected.expand();
                        selectionModel.select(node);
                    }
                }
            }, {
                xtype: 'button',
                text: 'Copy',
                handler: function() {
                    var tree = Ext.formbuilder.treePanel;
                    var selectionModel = tree.getSelectionModel();
                    var selection = selectionModel.getSelection();
                    if(selection.length > 0) {
                        Ext.formbuilder.treePanel.clipboard = selection[0];
                    }
                }
            }, {
                xtype: 'button',
                text: 'Paste',
                handler: function() {
                    var tree = Ext.formbuilder.treePanel;
                    var selectionModel = tree.getSelectionModel();
                    var selection = selectionModel.getSelection();
                    var source = Ext.formbuilder.treePanel.clipboard;
                    if(selection.length > 0 && source) {
                        var selected = selection[0];
                        var node = source.copy();
                        Ext.data.Model.id(node);
                        selected.appendChild(node);
                        selected.expand();
                        selectionModel.select(node);
                    }
                }
            }, {
                xtype: 'button',
                text: 'Delete',
                handler: function() {
                    var tree = Ext.formbuilder.treePanel;
                    var selectionModel = tree.getSelectionModel();
                    var selection = selectionModel.getSelection();
                    if(selection.length > 0) {
                        var selected = selection[0];
                        selected.remove(true);
                    }
                }
            }]
        },
        listeners: {
            selectionchange: function(view, selections) {
                if(selections.length > 0) {
                    var record = selections[0];
                    if(record.isRoot() || record.parentNode.isRoot()) {
                        Ext.formbuilder.showPropertiesForm();
                        return;
                    }
                    else {
                        // Load by name...
                        Ext.formbuilder.showElementForm();
                        var form = Ext.formbuilder.elementForm.getForm();
                        // record = Ext.clone(record);
                        var data = Ext.clone(record.data);
                        form.loadRecord(record);
                        //attributes
                        var form_grids = [ 'attributes', 'element_validate', 'process', 'pre_render', 'post_render', 'after_build', 'options', 'user_data', 'submit', 'validate'];
                        form_grids.forEach(function(name) {
                            Ext.getCmp(name).store.loadData(data[name], false);
                        });
                        /* Ahah */
                        var ahah = data.ahah;
                        if(ahah !== undefined && ahah != "") {
                            var values = {
                                ahah: "on",
                                ahah_effect: ahah.effect,
                                ahah_event: ahah.event,
                                ahah_method: ahah.method,
                                ahah_path: ahah.path,
                                ahah_wrapper: ahah.wrapper,
                                ahah_keypress: ahah.keypress
                            };
                            if(ahah.progress !== undefined && ahah.progress != "") {
                                var progress = ahah.progress;
                                values.ahah_progress = "on";
                                values.ahah_progress_type = progress.type;
                                values.ahah_progress_message = progress.message;
                                values.ahah_progress_url = progress.url;
                                values.ahah_progress_interval = progress.interval;
                            }
                            else {
                                Ext.getCmp('ahah_progress').collapse();
                            }
                            form.setValues(values);
                        }
                        else {
                            Ext.getCmp('ahah').collapse();
                            Ext.getCmp('ahah_progress').collapse();
                        }
                        var actions = data.actions;
                        if(actions !== undefined && actions != "") {
                            if(actions.create !== undefined && actions.create != "") {
                                var create = actions.create;
                                var values = {
                                    actions_create: "on",
                                    actions_create_context: create.context,
                                    actions_create_path: create.path
                                };
                                form.setValues(values);
                            }
                            else {
                                Ext.getCmp('actions_create').collapse();
                            }
                            if(actions.read !== undefined && actions.read != "") {
                                var read = actions.read;
                                var values = {
                                    actions_read: "on",
                                    actions_read_context: read.context,
                                    actions_read_path: read.path
                                };
                                form.setValues(values);
                            }
                            else {
                                Ext.getCmp('actions_read').collapse();
                            }
                            if(actions.update !== undefined && actions.update != "") {
                                var update = actions.update;
                                var values = {
                                    actions_update: "on",
                                    actions_update_context: update.context,
                                    actions_update_path: update.path
                                };
                                form.setValues(values);
                            }
                            else {
                                Ext.getCmp('actions_update').collapse();
                            }
                            if(actions['delete'] !== undefined && actions['delete'] != "") {
                                var remove = actions['delete'];
                                var values = {
                                    actions_delete: "on",
                                    actions_delete_context: remove.context,
                                    actions_delete_path: remove.path
                                };
                                form.setValues(values);
                            }
                            else {
                                Ext.getCmp('actions_delete').collapse();
                            }
                        }
                        else {
                            Ext.getCmp('actions_create').collapse();
                            Ext.getCmp('actions_read').collapse();
                            Ext.getCmp('actions_update').collapse();
                            Ext.getCmp('actions_delete').collapse();
                        }
                    }
                }
            }
        }
    });
}

