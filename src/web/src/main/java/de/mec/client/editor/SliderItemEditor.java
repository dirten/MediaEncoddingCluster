/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.sencha.gxt.widget.core.client.Slider;
import de.mec.client.PropertyItemEditor;
import de.mec.client.model.tools.String2IntegerConverter;

/**
 *
 * @author jhoelscher
 */
public class SliderItemEditor extends PropertyItemEditor{
    public SliderItemEditor() {
      super(new Slider(),new String2IntegerConverter());
    }
}
