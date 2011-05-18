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
            /* Create Tree Panel */
            this.treePanel = this.createTreePanel(); // Defined in TreePanel.js
            /* Create Main Panel */
            this.mainPanel = this.createMainPanel([this.treePanel, this.displayPanel]); // Defined in MainPanel.js
            this.createToolTips(); // Defined in Tooltips.js
        },
        /**
         * Creates an array based store.
         */
        createArrayStore: function () {
            return new Ext.data.Store({
                fields: ['value'],
                proxy: {
                    type: 'memory',
                    reader: {
                        type: 'array'
                    }
                },
                data: [["dummy value."]]
            });
        },
        /**
         * Creates a map based store.
         */
        createMapStore: function () {
            return new Ext.data.Store({
                fields: ['key', 'value'],
                proxy: {
                    type: 'memory',
                    reader: {
                        type: 'json'
                    }
                },
                data: [{
                    key: 'dummy key', 
                    value:'dummy value'
                }]
            });
        },
        /**
         *
         */
        showPreview: function () {
            var display = this.displayPanel.layout;
            display.setActiveItem(0);
        },
        /**
         *
         */
        showElementForm: function () {
            var display = this.displayPanel.layout;
            display.setActiveItem(1);
        },
        /**
         *
         */
        showPropertiesForm: function () {
            var display = this.displayPanel.layout;
            display.setActiveItem(2);
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
