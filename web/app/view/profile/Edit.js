Ext.define('MEC.view.profile.Edit', {
               extend: 'Ext.window.Window',
               alias: 'widget.profileedit',

               title: 'Edit Profile',
               layout: 'fit',
               autoShow: true,
               width:600,
               items : [
                           {
                               xtype: 'form',
                               layout : 'vbox',
                               flex:1,
                               fieldDefaults:{
                                   labelAlign:'top',
                                   msgTarget:'side'
                               },
                               items: [
                                          {
                                              xtype:'container',
                                              layout:'anchor',

                                              items:[
                                                        {
                                                            xtype: 'textfield',
                                                            width:400,
                                                            name : 'uuid',
                                                            fieldLabel: 'ID'
                                                        },
                                                        {
                                                            xtype: 'textfield',
                                                            width:400,
                                                            name : 'name',
                                                            fieldLabel: 'Name'
                                                        }]
                                          },
                                          {
                                              xtype:'tabpanel',
                                              activeTab:0,
                                              width:600,
                                              border:false,
                                              items:[
                                                        {
                                                            title:'first',
                                                            items:[
                                                                      {
                                                                          xtype: 'textfield',
                                                                          fieldLabel:'bla',
                                                                          name:'audio.id'


                                                                      }

                                                                  ]
                                                        },
                                                        {
                                                            title:'second'
                                                        }

                                                    ]
                                          }
                                      ]
                           }
                       ],
               initComponent: function() {

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
