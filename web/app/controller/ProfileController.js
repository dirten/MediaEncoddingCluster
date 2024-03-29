Ext.define('MEC.controller.ProfileController', {
               extend: 'Ext.app.Controller',

               views:[
                         'profile.List',
                         'profile.Edit'
                     ],
               stores: [
                           'ProfileStore'
                       ],
               models:['Profile','Audio'],
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

                             //console.log(record.get("uuid"));
                             var Profile = Ext.ModelMgr.getModel('MEC.model.Profile');


                             Profile.load(record.get("uuid"),{
                                              success: function(profile) {
                                                           //var view2=this.getProfileEdit();
                                                           var view = Ext.widget('profileedit');
                                                           //var view = Ext.widget('profiletop');
                                                           console.log(profile);
                                                           console.log(profile.getAudio());
                                                           //console.log(view);
                                                           //console.log(view.down('form'));
                                                           //profile.data['audioid']=profile.data.audio.id;
                                                           //profile.set('audio.id','data');
                                                           //console.log(profile.getData());
                                                           //profile.getData().audio
                                                           view.down('form').loadRecord(profile);
                                                           //var view = Ext.widget('audiopanel');
                                                           view.down('form').loadRecord(profile.getAudio());
                                                           //view.down('form').loadRecord(profile.get('audio'));
                                                           //view.loadRecord(profile);
                                                           //view.getForm().loadRecord(profile);
                                                           //view.down('form').setValue('audio','bla');
                                                           //view.down('form').findField('audiofield').setValue('hallo');
                                                       }
                                          });


                         },

               updateFormat: function(button) {
                                 //console.log('clicked the Save button');
                                 var win    = button.up('window'),
                                         form   = win.down('form'),
                                         record = form.getRecord(),
                                         values = form.getValues();

                                 record.set(values);
                                 console.log(record);
                                 var store = this.getProfileStoreStore();
                                 record.save({
                                                 success: function(ed) {
                                                              //console.log("Saved Ed! His ID is "+ ed);
                                                              store.load();
                                                          }
                                             });
                                 win.close();
                             }
           }
           );
