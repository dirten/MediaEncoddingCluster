/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import de.mec.client.PropertyItemEditor;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class ItemEditorFactory {

    private static final HashMap<String, Class> PROPERTY_CLASS_MAP = new HashMap<String, Class>();

    static {
        PROPERTY_CLASS_MAP.put("b", SliderItemEditor.class);
        PROPERTY_CLASS_MAP.put("left", TextItemEditor.class);
        PROPERTY_CLASS_MAP.put("bla", CheckboxItemEditor.class);
        PROPERTY_CLASS_MAP.put("second", CheckboxItemEditor.class);
    }

    private ItemEditorFactory() {
    }

    public static PropertyItemEditor getEditor(final String property) {
        if (PROPERTY_CLASS_MAP.containsKey(property)) {
            if (PROPERTY_CLASS_MAP.get(property).equals(SliderItemEditor.class)) {
                return new SliderItemEditor();
            } else if (PROPERTY_CLASS_MAP.get(property).equals(TextItemEditor.class)) {
                return new TextItemEditor();
            } else if (PROPERTY_CLASS_MAP.get(property).equals(CheckboxItemEditor.class)) {
                return new CheckboxItemEditor();
            }
        }
        return new TextItemEditor();
    }
    
    public static PropertyItemEditor getEditorByClassName(final String property) {
      if("TextBox".equals(property)){
        return new TextItemEditor();
      }
      if("ComboBox".equals(property)){
        return new TextItemEditor();
      }
      if("CheckBox".equals(property)){
        return new CheckboxItemEditor();
      }
      if("Slider".equals(property)){
        return new SliderItemEditor();
      }
      return new TextItemEditor();
    }
}
