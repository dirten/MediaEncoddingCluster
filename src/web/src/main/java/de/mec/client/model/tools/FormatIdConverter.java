/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.model.tools;

import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.data.shared.Store;
import de.mec.client.model.Format;

/**
 *
 * @author jhoelscher
 */
public class FormatIdConverter implements Converter<String, Format> {

  private final Store<Format> store;

  public FormatIdConverter(Store<Format> store) {
    this.store = store;
  }

  @Override
  public String convertFieldValue(Format object) {
    if (object == null) {
      return "";
    }
    return object.getId();
  }

  @Override
  public Format convertModelValue(String object) {
    return store.findModelWithKey(object);
  }
}
