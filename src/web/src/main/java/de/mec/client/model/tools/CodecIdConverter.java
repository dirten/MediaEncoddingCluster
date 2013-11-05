/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.data.shared.Store;
import de.mec.client.model.Codec;

/**
 *
 * @author jan.hoelscher
 */
public class CodecIdConverter implements Converter<String, Codec>{
    private final Store<Codec> store;

        public CodecIdConverter(Store<Codec> store) {
            this.store = store;
        }

        @Override
        public String convertFieldValue(Codec object) {
            System.out.println("Codec="+object);
          if(object==null)return "";
            return object.getId();
        }

        @Override
        public Codec convertModelValue(String object) {
            System.out.println("Object="+object);
            return store.findModelWithKey(object);
        }
}
