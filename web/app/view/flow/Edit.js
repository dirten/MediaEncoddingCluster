Ext.define('MEC.view.flow.Edit', {
    extend: 'Ext.window.Window',
    alias: 'widget.flowedit',

    title: 'Edit Flow',
    layout: 'fit',
    autoShow: true,

    initComponent: function() {
        this.items = [
            {
                xtype: 'form',
                items: [
                    {
                        xtype: 'textfield',
                        name : 'uuid',
                        fieldLabel: 'ID'
                    },
                    {
                        xtype: 'textfield',
                        name : 'name',
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
