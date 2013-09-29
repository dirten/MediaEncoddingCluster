Ext.define('MEC.store.Formats', {
               extend: 'Ext.data.Store',
               model:'MEC.model.Format',
               autoLoad: true,
               api: {
                       read: 'api/v1/format',
                       update: 'data/updateUsers.json'
                   },
               proxy: {
                   type: 'ajax',
                   url: 'api/v1/format',
                   reader: {
                       type: 'json',
                       root: 'data',
                       successProperty: 'success'
                   }
               }});
