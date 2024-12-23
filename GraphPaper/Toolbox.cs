using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace GraphPaper
{
    interface IClickTool<T>
    {
        void Gesturing(T context, Point p);
        void Gestured(T context, Point p);
        string HelpText { get; }
    }

    interface IDragTool<T>
    {
        void Gesturing(T context, Point start, Point current);
        void Gestured(T context, Point start, Point end);
        string HelpText { get; }
    }

    interface IToolbox<T>
    {
        IClickTool<T> GetClickTool(Keys modifierKeys, MouseButtons button);
        IDragTool<T> GetDragTool(Keys modifierKeys, MouseButtons button);
        string HelpText { get; }
    }

    class Toolbox<T> : IToolbox<T>
    {
        SortedDictionary<Keys, SortedDictionary<MouseButtons, IClickTool<T>>> clickTools;
        SortedDictionary<Keys, SortedDictionary<MouseButtons, IDragTool<T>>> dragTools;
        IClickTool<T> missingClickTool;
        IDragTool<T> missingDragTool;
        public string HelpText { get; private set; }

        public Toolbox()
        {
            clickTools = new SortedDictionary<
                Keys, SortedDictionary<MouseButtons, IClickTool<T>>>();
            dragTools = new SortedDictionary<
                Keys, SortedDictionary<MouseButtons, IDragTool<T>>>();
            HelpText = string.Empty;
        }

        public void SetMissingClickTool(IClickTool<T> clickTool)
        {
            missingClickTool = clickTool;
        }

        public void SetMissingDragTool(IDragTool<T> dragTool)
        {
            missingDragTool = dragTool;
        }

        public void SetClickTool(
            Keys modifierKeys, MouseButtons button, IClickTool<T> clickTool)
        {
            if (false == clickTools.ContainsKey(modifierKeys))
                clickTools.Add(modifierKeys,
                    new SortedDictionary<MouseButtons, IClickTool<T>>());

            clickTools[modifierKeys][button] = clickTool;

            if (!string.IsNullOrEmpty(HelpText))
                HelpText += Environment.NewLine;
            if (modifierKeys != Keys.None)
                HelpText += modifierKeys + " ";
            HelpText += button + " click: " + clickTool.HelpText;
        }

        public void SetDragTool(
            Keys modifierKeys, MouseButtons button, IDragTool<T> dragTool)
        {
            if (false == dragTools.ContainsKey(modifierKeys))
                dragTools.Add(modifierKeys,
                    new SortedDictionary<MouseButtons, IDragTool<T>>());

            dragTools[modifierKeys][button] = dragTool;

            if (!string.IsNullOrEmpty(HelpText))
                HelpText += Environment.NewLine;
            if (modifierKeys != Keys.None)
                HelpText += modifierKeys + " ";
            HelpText += button + " drag: " + dragTool.HelpText;
        }

        public IClickTool<T> GetClickTool(Keys modifierKeys, MouseButtons button)
        {
            if (false == clickTools.ContainsKey(modifierKeys) ||
                false == clickTools[modifierKeys].ContainsKey(button))
                return missingClickTool;

            return clickTools[modifierKeys][button];
        }

        public IDragTool<T> GetDragTool(Keys modifierKeys, MouseButtons button)
        {
            if (false == dragTools.ContainsKey(modifierKeys) ||
                false == dragTools[modifierKeys].ContainsKey(button))
                return missingDragTool;

            return dragTools[modifierKeys][button];
        }
    }
}
