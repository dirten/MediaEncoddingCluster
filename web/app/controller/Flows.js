Ext.define('MEC.controller.Flows', {
               extend: 'Ext.app.Controller',

               views:[
                   'flow.List',
                   'flow.Edit'
               ],
               stores: [
                   'Flows'
               ],
               models:['Flow'],
               init: function() {
                   this.control({
                                    'flowlist': {
                                        itemdblclick: this.editFlow
                                    },
                                    'flowedit button[action=save]': {
                                        click: this.updateFormat
                                    }
                                });
               },

               editFlow: function(grid, record) {
                   var view = Ext.widget('flowedit');
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
                   this.getFlowsStore().sync();
               }
           }
           );
