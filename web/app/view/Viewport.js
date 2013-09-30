Ext.define('MEC.view.Viewport', {
               extend: 'Ext.container.Viewport',
               layout: 'fit',

               requires: [
                   'MEC.view.flow.List',
                   'MEC.view.format.List',
                   'MEC.view.profile.List'
               ],

               initComponent: function() {
                   this.items = {
                       dockedItems: [{
                               dock: 'top',
                               xtype: 'toolbar',
                               height: 80,
                               /*
                items: [{
                    xtype: 'newstation',
                    width: 150
                }, {
                    xtype: 'songcontrols',
                    flex: 1
                }, {
                    xtype: 'component',
                    html: 'Pandora<br>Internet Radio'
                }]*/
                           }],
                       layout: {
                           type: 'hbox',
                           align: 'stretch'
                       },
                       items: [{
                               width: 250,
                               xtype: 'panel',
                               layout: {
                                   type: 'vbox',
                                   align: 'stretch'
                               },
                               items: [{
                                       xtype: 'profilelist',
                                       flex: 1
                                   }/*, {
                    html: 'Ad',
                    height: 150,
                    xtype: 'panel'
                }*/]
                           }, {
                               xtype: 'container',
                               flex: 1,
                               border: false,
                               layout: {
                                   type: 'vbox',
                                   align: 'stretch'
                               },
                               items: [{
                                       xtype: 'recentlyplayedscroller',
                                       height: 250
                                   }, {
                                       xtype: 'songinfo',
                                       flex: 1
                                   }]
                           }]
                   };

                   this.callParent();
               }
           });
