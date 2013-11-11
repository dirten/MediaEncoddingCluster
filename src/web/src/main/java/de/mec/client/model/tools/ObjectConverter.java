/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;

/**
 *
 * @author jhoelscher
 */
public class ObjectConverter<T> implements Converter<T, T>{

  public T convertFieldValue(T object) {
            System.out.println("Object="+object);
    return object;
  }

  public T convertModelValue(T object) {
            System.out.println("Object="+object);
    return object;
  }
  
}
