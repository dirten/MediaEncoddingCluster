Ext.define('MEC.controller.Formats', {
               extend: 'Ext.app.Controller',

               views:[
                   'format.List',
                   'format.Edit'
               ],
               stores: [
                   'Formats'
               ],
               models:['Format'],
               init: function() {
                   this.control({
                                    'viewport > formatlist': {
                                        itemdblclick: this.editFormat
                                    },
                                    'formatedit button[action=save]': {
                                        click: this.updateFormat
                                    }
                                });
               },

               editFormat: function(grid, record) {
                   var view = Ext.widget('formatedit');
                   view.down('form').loadRecord(record);
                   console.log('Double clicked on ' + record.get('name'));
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
                   this.getFormatsStore().sync();
               }
           }
           );
