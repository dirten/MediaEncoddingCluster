Ext.define('MEC.view.format.Edit', {
    extend: 'Ext.window.Window',
    alias: 'widget.formatedit',

    title: 'Edit Format',
    layout: 'fit',
    autoShow: true,

    initComponent: function() {
        this.items = [
            {
                xtype: 'form',
                items: [
                    {
                        xtype: 'textfield',
                        name : 'id',
                        fieldLabel: 'ID'
                    },
                    {
                        xtype: 'textfield',
                        name : 'longname',
                        fieldLabel: 'Name'
                    }
                ]
            }
        ];

        this.buttons = [
            {
                text: 'Save',
                action: 'save'
            },
            {
                text: 'Cancel',
                scope: this,
                handler: this.close
            }
        ];

        this.callParent(arguments);
    }
});
