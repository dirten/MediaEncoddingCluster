Ext.define('MEC.view.profile.List' ,{
               extend: 'Ext.grid.Panel',
               alias: 'widget.profilelist',

               title: 'All Profiles',
               store : 'ProfileStore',
               hideHeaders:true,
               initComponent: function() {

                   this.columns = [
                            {header: 'Name', dataIndex: 'name', flex: 1}
                        ];

                   this.dockedItems = [{
                                           dock: 'bottom',
                                           xtype: 'toolbar',
                                           items: [{
                                                   xtype: 'button',
                                                   text: 'Settings',
                                                   action: 'settings'
                                               }, {
                                                   xtype: 'buttongroup',
                                                   items: [{
                                                           xtype: 'button',
                                                           text: 'By Date',
                                                           action: 'filter-date'
                                                       }, {
                                                           xtype: 'button',
                                                           text: 'ABC',
                                                           action: 'filter-name'
                                                       }]
                                               }]
                                       }];
                   this.callParent(arguments);

               }

           });
