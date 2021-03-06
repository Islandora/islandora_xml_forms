/*

This file is part of Ext JS 4

Copyright (c) 2011 Sencha Inc

Contact:  http://www.sencha.com/contact

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

If you are unsure which license is appropriate for your use, please contact the sales department at http://www.sencha.com/contact.

*/
/**
 * @class Ext.layout.container.VBox
 * @extends Ext.layout.container.Box
 * <p>A layout that arranges items vertically down a Container. This layout optionally divides available vertical
 * space between child items containing a numeric <code>flex</code> configuration.</p>
 * This layout may also be used to set the widths of child items by configuring it with the {@link #align} option.
 * {@img Ext.layout.container.VBox/Ext.layout.container.VBox.png Ext.layout.container.VBox container layout}
 * Example usage:
	Ext.create('Ext.Panel', {
		width: 500,
		height: 400,
		title: "VBoxLayout Panel",
		layout: {                        
			type: 'vbox',
			align: 'center'
		},
		renderTo: document.body,
		items: [{                        
			xtype: 'panel',
			title: 'Inner Panel One',
			width: 250,
			flex: 2                      
		},{
			xtype: 'panel',
			title: 'Inner Panel Two',
			width: 250,			
			flex: 4
		},{
			xtype: 'panel',
			title: 'Inner Panel Three',
			width: '50%',			
			flex: 4
		}]
	});
 */
Ext.define('Ext.layout.container.VBox', {

    /* Begin Definitions */

    alias: ['layout.vbox'],
    extend: 'Ext.layout.container.Box',
    alternateClassName: 'Ext.layout.VBoxLayout',
    
    /* End Definitions */

    /**
     * @cfg {String} align
     * Controls how the child items of the container are aligned. Acceptable configuration values for this
     * property are:
     * <div class="mdetail-params"><ul>
     * <li><b><tt>left</tt></b> : <b>Default</b><div class="sub-desc">child items are aligned horizontally
     * at the <b>left</b> side of the container</div></li>
     * <li><b><tt>center</tt></b> : <div class="sub-desc">child items are aligned horizontally at the
     * <b>mid-width</b> of the container</div></li>
     * <li><b><tt>stretch</tt></b> : <div class="sub-desc">child items are stretched horizontally to fill
     * the width of the container</div></li>
     * <li><b><tt>stretchmax</tt></b> : <div class="sub-desc">child items are stretched horizontally to
     * the size of the largest item.</div></li>
     * </ul></div>
     */
    align : 'left', // left, center, stretch, strechmax

    //@private
    alignCenteringString: 'center',

    type: 'vbox',

    direction: 'vertical',

    // When creating an argument list to setSize, use this order
    parallelSizeIndex: 1,
    perpendicularSizeIndex: 0,

    parallelPrefix: 'height',
    parallelPrefixCap: 'Height',
    parallelLT: 't',
    parallelRB: 'b',
    parallelBefore: 'top',
    parallelBeforeCap: 'Top',
    parallelAfter: 'bottom',
    parallelPosition: 'y',

    perpendicularPrefix: 'width',
    perpendicularPrefixCap: 'Width',
    perpendicularLT: 'l',
    perpendicularRB: 'r',
    perpendicularLeftTop: 'left',
    perpendicularRightBottom: 'right',
    perpendicularPosition: 'x',
    configureItem: function(item) {
        if (item.flex) {
            item.layoutManagedHeight = 1;
        } else {
            item.layoutManagedHeight = 2;
        }

        if (this.align === 'stretch' || this.align === 'stretchmax') {
            item.layoutManagedWidth = 1;
        } else {
            item.layoutManagedWidth = 2;
        }
        this.callParent(arguments);
    }
});
