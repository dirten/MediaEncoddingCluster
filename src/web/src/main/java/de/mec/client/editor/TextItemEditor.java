/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.PropertyItemEditor;

/**
 *
 * @author jhoelscher
 */
public class TextItemEditor extends PropertyItemEditor{
 
    public TextItemEditor() {
      super(new TextField(), null);
    }
}
