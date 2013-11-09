/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.web.bindery.autobean.shared.AutoBean;
import de.mec.client.model.Profile;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class ProfileCategory {
    private static final Logger LOG = Logger.getLogger(ProfileCategory.class.getName());
    public static List<PropertyItem> getVideoPropertyItemList(AutoBean<Profile> instance) {
        LOG.log(Level.INFO,"calling category:");
          List<PropertyItem>result=new ArrayList<PropertyItem>();
          /*
          for(Map.Entry<String, String> entry:instance.as().getVideoMap().entrySet()){
              PropertyItem item=new PropertyItem(entry.getKey(), entry.getValue());
              result.add(item);
          }*/
          LOG.log(Level.INFO,"prepare result in category:"+result);
          return result;
      }
}
