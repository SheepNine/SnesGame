using System.Collections.Generic;
using System.Linq;

namespace Patchwork
{
    public class MruList
    {
        List<string> recentArrangments;

        public MruList(IEnumerable<string> values)
        {
            recentArrangments = values.Distinct().ToList();
        }

        public int Count
        {
            get { return recentArrangments.Count; }
        }

        public string this[int index]
        {
            get { return recentArrangments[index]; }
        }

        public void AddToList(string newItem)
        {
            recentArrangments.Remove(newItem);
            recentArrangments.Insert(0, newItem);
        }

        public void RemoveFromList(string filename)
        {
            recentArrangments.Remove(filename);
        }
    }
}
