Ext.define('MEC.view.template.Edit', {
    extend: 'Ext.XTemplate',

    constructor: function (config) {
        this.callParent(['<div style="padding-left: 20px;">',
                                    '<div style="width:100%;float:left;position:relative">{label}bla fasel</div>',
                                    '<img src="{icon}" style="float:left;position:relative;width:20px;right:20px;margin-left:-100%;height:20px;" />',
                                  '</div>']
                                  );
    }
}) ;

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
                                                            title:'second',
                                                            items:[{
                                                                       xtype:'panel',
                                                                       html:Ext.create('Ext.XTemplate', 'first {firstName} last{lastName}').apply({firstName:'jon'})

                                                            }]
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

                                  //this.tpl.overwrite(this.getEl(),{firstName:"bla"});

                                  this.callParent(arguments);
                              }
           });
