Ext.define('MEC.store.ProfileStore', {
               extend: 'Ext.data.Store',
               model:'MEC.model.Profile',
               autoLoad: true,
               api: {
                       read: 'api/v1/profile',
                       update: 'data/updateUsers.json'
                   }
               /*,
               proxy: {
                   type: 'ajax',
                   url: 'api/v1/profile',
                   reader: {
                       type: 'json',
                       root: 'data',
                       successProperty: 'success'
                   }
               }*/
           }
           );
