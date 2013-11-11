/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;
import de.mec.client.PropertyItem;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

/**
 *
 * @author jhoelscher
 */
public class PropertyItemListConverter implements Converter<Map<String,String>, List<PropertyItem>>{

  public Map<String, String> convertFieldValue(List<PropertyItem> object) {
    Map<String, String>result=new HashMap<String,String>();
    for(PropertyItem item:object){
      result.put(item.getKey(), item.getVal());
    }
    return result;
  }

  public List<PropertyItem> convertModelValue(Map<String, String> object) {
    List<PropertyItem>result=new ArrayList<PropertyItem>();
    for(Entry<String, String>entry:object.entrySet()){
      result.add(new PropertyItem(entry.getKey(), entry.getValue()));
    }
    
    return result;
  }
  
}
