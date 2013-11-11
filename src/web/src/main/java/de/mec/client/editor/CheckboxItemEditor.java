/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.sencha.gxt.widget.core.client.form.CheckBox;
import de.mec.client.PropertyItemEditor;
import de.mec.client.model.tools.StringBooleanConverter;

/**
 *
 * @author jan.hoelscher
 */
public class CheckboxItemEditor extends PropertyItemEditor{
    public CheckboxItemEditor(){
         super(new CheckBox(),new StringBooleanConverter());
    }
}
