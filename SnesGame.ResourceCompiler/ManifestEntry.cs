using System.IO;
using System.Text.RegularExpressions;

namespace SnesGame.ResourceCompiler
{
    public abstract class ManifestEntry
    {
        public string ID { get; private set; }

        public ManifestEntry(string tag, string id)
        {
            if (string.IsNullOrEmpty(id))
                throw new InvalidDataException(string.Format("A {0} is missing an 'id' attribute", tag));
            if (!Regex.IsMatch(id, "[A-Z][A-Z0-9_]+"))
                throw new InvalidDataException(string.Format("A {0} has invalid 'id' attribute '{1}'", tag, id));

            ID = id;
        }
    }
}
