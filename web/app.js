Ext.application({
                    requires: ['Ext.container.Viewport'],
                    name: 'MEC',

                    appFolder: 'app',
                    controllers: [
                        'Formats'
                    ],
                    views: [
                            'format.List'
                        ],
                    launch: function() {
                        Ext.create('Ext.container.Viewport', {
                                       layout: 'fit',
                                       items: [
                                           {
                                               xtype: 'formatlist',
                                               title: 'Formats',
                                               html : 'List of users will go here12'
                                           }
                                       ]

                                   });
                    }
                });
