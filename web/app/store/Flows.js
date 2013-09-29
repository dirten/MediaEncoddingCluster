Ext.define('MEC.store.Flows', {
               extend: 'Ext.data.Store',
               model:'MEC.model.Flow',
               autoLoad: true,
               api: {
                       read: 'api/v1/flow',
                       update: 'data/updateUsers.json'
                   },
               proxy: {
                   type: 'ajax',
                   url: 'api/v1/flow',
                   reader: {
                       type: 'json',
                       root: 'data',
                       successProperty: 'success'
                   }
               }});
