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
public class String2IntegerConverter implements Converter<String, Integer> {

  public String convertFieldValue(Integer object) {
    System.out.println("String2IntegerConverter::convertFieldValue=" + object);
    if (object == null) {
      return "0";
    }

    return object.toString();
  }

  public Integer convertModelValue(String object) {
    System.out.println("String2IntegerConverter::convertModelValue=" + object+" class:"+object.getClass());
    Integer result=0;
    try{
      result=Integer.parseInt(object);
    }catch(Exception ex){
      
    }
    return result; 
  }
}
