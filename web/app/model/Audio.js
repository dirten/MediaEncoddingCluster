Ext.define('MEC.model.Audio', {
               extend: 'Ext.data.Model',
               fields: [
                           { name:'id', type:'auto'},
                           { name:'ac', type:'auto' },
                           { name:'ab', type:'auto' },
                           { name:'ar', type:'auto' }
                       ],
               belongsTo:'Profile'

           });
