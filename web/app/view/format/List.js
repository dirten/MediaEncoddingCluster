Ext.define('MEC.view.format.List' ,{
    extend: 'Ext.grid.Panel',
    alias: 'widget.formatlist',

    title: 'All Formats',

    initComponent: function() {
        this.store = 'Formats';

        this.columns = [
            {header: 'ID',  dataIndex: 'id',  flex: 1},
            {header: 'Name', dataIndex: 'longname', flex: 1}
        ];

        this.callParent(arguments);
    }
});
