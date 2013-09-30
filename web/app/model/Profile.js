Ext.define('MEC.model.Profile', {
               extend: 'Ext.data.Model',
               fields: [
                           { name:'uuid', type:'auto'},
                           { name:'name', type:'auto' },
                           { name:'format', type:'auto' },
                           { name:'video', type:'auto' },
                           { name:'audio', type:'auto' },
                           { name:'audio.id', mapping:'audio.id'}
                       ],

               proxy: {
                   type: 'rest',
                   url: 'api/v1/profile',
                   actionMethods : {
                       create : 'POST',
                       read   : 'GET',
                       update : 'PUT',
                       destroy: 'DELETE'
                      },
                   reader: {
                       type: 'json',
                       root: 'data',
                       successProperty: 'success'
                   }
               }
           });
