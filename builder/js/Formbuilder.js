/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 * Create Namespace for this application.
 */
Ext.ns('Ext.formbuilder');

/**
 * Application Object.
 */
Ext.formbuilder = (function() {
    var url = window.location.pathname; // Private Variable.
    var that = {
        /**
         * Create all the components required to render this application.
         */
        create: function() {
            /* Create Display Panel: Card Layout */
            this.propertiesForm = this.createPropertiesForm(); // Defined in PropertiesForm.js
            this.elementForm = this.createElementForm(); // Defined in ElementForm.js
            this.previewPanel = this.createPreviewPanel(url); // Defined in PreviewPanel.js
            this.displayPanel = this.createDisplayPanel([this.previewPanel, this.elementForm, this.propertiesForm]); // Defined in DisplayPanel.js
            this.displayPanel.activeItem = 0;// TODO: remove.
            /* Create Tree Panel */
            this.treePanel = this.createTreePanel(); // Defined in TreePanel.js
            this.treePanel.expandAll();
            /* Create Main Panel */
            this.mainPanel = this.createMainPanel([this.treePanel, this.displayPanel]); // Defined in MainPanel.js
        //this.createToolTips(); // Defined in Tooltips.js
        },
        /**
         * Creates an array based store.
         */
        createArrayStore: function () {
            return new Ext.data.Store({
                model: 'ArrayModel'
            });
        },
        /**
         * Creates a map based store.
         */
        createMapStore: function () {
            return new Ext.data.Store({
                model: 'MapModel'
            });
        },
        /**
         *
         */
        showPreview: function () {
            this.refreshPreviewPanel(url);
        },
        /**
         *
         */
        showElementForm: function () {
            var display = this.displayPanel.layout;
            display.setActiveItem(1);
            var element = Ext.getCmp('xml-form-builder-element-form-tab-panel');
            element.setActiveTab(0);
        },
        /**
         *
         */
        showPropertiesForm: function () {
            var display = this.displayPanel.layout;
            display.setActiveItem(2);
            var record = Ext.formbuilder.propertiesStore.getAt(0);
            var form = this.propertiesForm.getForm();
            form.loadRecord(record);
            var namespaces = [];
            if(record.data['namespaces'] instanceof Object) {
                $.each(record.data['namespaces'], function(i, n) {
                    namespaces.push({
                        key: i, 
                        value: n
                    });
                });
            }
            Ext.getCmp('namespaces').store.loadData(namespaces, false);
        }
    };
    return that;
})();

/**
 * Run the application.
 */
Ext.onReady(function() {
    Ext.formbuilder.create();
});
