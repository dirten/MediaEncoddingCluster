Ext.define('MEC.controller.Formats', {
               extend: 'Ext.app.Controller',

               views:[
                   'format.List'
               ],
               init: function() {
                   this.control({
                                    'formatlist': {
                                        itemdblclick: this.editFormat
                                    }
                                });
               },

               editFormat: function(grid, record) {
                   console.log('Double clicked on ' + record.get('name'));
               }
           }
           );
