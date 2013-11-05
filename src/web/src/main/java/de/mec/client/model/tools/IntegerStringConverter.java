/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;

/**
 *
 * @author jan.hoelscher
 */
public class IntegerStringConverter implements Converter<Integer, String>{

        public IntegerStringConverter() {
        }

        @Override
        public Integer convertFieldValue(String object) {
            return Integer.parseInt(object);
        }

        @Override
        public String convertModelValue(Integer object) {
          if(object==null)return "0";
            return object.toString();
        }
}
