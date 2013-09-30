Ext.require('Ext.data.Store');
Ext.define('MEC.controller.ProfileController', {
               extend: 'Ext.app.Controller',

               views:[
                   'profile.List',
                   'profile.Edit'
               ],
               stores: [
                   'ProfileStore'
               ],
               models:['Profile'],
               init: function() {
                   this.control({
                                    'profilelist': {
                                        itemdblclick: this.editFlow
                                    },
                                    'profileedit button[action=save]': {
                                        click: this.updateFormat
                                    }
                                });
               },

               editFlow: function(grid, record) {
                   /*
                   userStore = Ext.create('Ext.data.Store', {
                           model: 'Profile',
                           autoLoad: true
                       });
                   var Profile = Ext.ModelMgr.getModel('Profile');
                   console.log(record.get("id"));
                   Profile.load(record.get("id"),{
                                    success: function(profile) {
                                            console.log("Loaded user 1: " + user.get('name'));
                                        }
                                });
                   */
                   var view = Ext.widget('profileedit');
                   view.down('form').loadRecord(record);
                   console.log(record);
               },
               updateFormat: function(button) {
                   console.log('clicked the Save button');
                   var win    = button.up('window'),
                           form   = win.down('form'),
                           record = form.getRecord(),
                           values = form.getValues();

                   record.set(values);
                   console.log(record);
                   win.close();
                   this.getFlowsStore().sync();
               }
           }
           );
