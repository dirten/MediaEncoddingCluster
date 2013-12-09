Ext.application(
        {
            requires: ['Ext.container.Viewport'],
            name: 'MEC',
            autoCreateViewport:true,
            appFolder: 'app',
            controllers: [
                'Flows',
                'Formats',
                'ProfileController'
            ],
            /*
                    views: [
                        'flow.List',
                        'format.List'
                    ],
                    launch: function() {
                        Ext.create('Ext.container.Viewport', {
                                       layout: 'fit',
                                       items: [
                                           {
                                               xtype: 'flowlist',
                                               title: 'Flows',
                                               html : 'List of users will go here12'
                                           }
                                       ]

                                   });
                    }*/
        }
        );
