using System;
using System.Drawing;
using UnaryHeap.DataType;

namespace GraphPaper
{
    class ModelViewTransform : ICloneable
    {
        public event EventHandler TransformChanged;
        protected void OnTransformChanged()
        {
            if (null != TransformChanged)
                TransformChanged(this, EventArgs.Empty);
        }

        public Point2D ModelCenter { get; private set; }
        Rational modelHeight;
        Rectangle viewExtents;

        Matrix3D modelToView;
        Matrix3D viewToModel;

        public ModelViewTransform()
        {
            this.ModelCenter = Point2D.Origin;
            this.viewExtents = new Rectangle(0, 0, 1, 1);
            this.modelHeight = new Rational(1, 2);

            InitMatrices();
        }

        public Orthotope2D ModelExtents
        {
            get
            {
                return Orthotope2D.FromPoints(new[] {
                    ModelFromView(new Point2D(viewExtents.Left, viewExtents.Top)),
                    ModelFromView(new Point2D(viewExtents.Right, viewExtents.Bottom))
                });
            }
        }

        public void UpdateModelCenter(Point2D newCenter)
        {
            this.ModelCenter = newCenter;

            InitMatrices();
        }

        public void UpdateViewport(Rectangle newViewExtents)
        {
            modelHeight = modelHeight * newViewExtents.Height / viewExtents.Height;
            viewExtents = newViewExtents;

            InitMatrices();
        }

        public void UpdateModelRange(Orthotope2D newExtents, Rational padding)
        {
            this.ModelCenter = newExtents.Center;
            this.modelHeight = FitRange(newExtents, padding);

            InitMatrices();
        }

        public void ZoomIn()
        {
            this.modelHeight *= new Rational(2, 3);
            InitMatrices();
        }

        public void ZoomOut()
        {
            this.modelHeight *= new Rational(3, 2);
            InitMatrices();
        }

        Rational FitRange(Orthotope2D modelRange, Rational padding)
        {
            var y = modelRange.Y.Size;
            var x = modelRange.X.Size * viewExtents.Height / viewExtents.Width;

            return Rational.Max(x, y) * padding;
        }

        void InitMatrices()
        {
            var modelWidth = modelHeight * viewExtents.Width / viewExtents.Height;


            modelToView = AffineMapping.From(
                new Point2D(ModelCenter.X - modelWidth / 2, ModelCenter.Y + modelHeight / 2),
                new Point2D(ModelCenter.X + modelWidth / 2, ModelCenter.Y + modelHeight / 2),
                new Point2D(ModelCenter.X - modelWidth / 2, ModelCenter.Y - modelHeight / 2))
                .Onto(
                new Point2D(viewExtents.Left, viewExtents.Top),
                new Point2D(viewExtents.Right, viewExtents.Top),
                new Point2D(viewExtents.Left, viewExtents.Bottom)
                );

            viewToModel = modelToView.ComputeInverse();

            OnTransformChanged();
        }

        public Orthotope2D ModelFromView(Rectangle rect)
        {
            return Orthotope2D.FromPoints(new[] {
                ModelFromView(new Point2D(rect.Left, rect.Top)),
                ModelFromView(new Point2D(rect.Right, rect.Bottom))
            });
        }

        public RectangleF ViewFromModel(Orthotope2D modelArea)
        {
            var upperLeft = ViewFromModel(
                new Point2D(modelArea.X.Min, modelArea.Y.Max));
            var lowerRight = ViewFromModel(
                new Point2D(modelArea.X.Max, modelArea.Y.Min));

            return new RectangleF(
                (float)upperLeft.X, (float)upperLeft.Y,
                (float)(lowerRight.X - upperLeft.X),
                (float)(lowerRight.Y - upperLeft.Y));
        }

        public Point2D ViewFromModel(Point2D modelCoords)
        {
            return Transform(modelCoords.X, modelCoords.Y, modelToView);
        }

        public Point2D ModelFromView(Point2D viewCoords)
        {
            return Transform(viewCoords.X, viewCoords.Y, viewToModel);
        }

        public Point2D ModelFromView(Point viewCoords)
        {
            return Transform(viewCoords.X, viewCoords.Y, viewToModel);
        }

        static Point2D Transform(Rational x, Rational y, Matrix3D m)
        {
            var affineCoords = new Point3D(x, y, 1);
            var affineResult = m * affineCoords;
            return new Point2D(
                affineResult.X / affineResult.Z,
                affineResult.Y / affineResult.Z);
        }

        public Rational Quadrance(Point viewPoint1, Point viewPoint2)
        {
            return Point2D.Quadrance(
                ModelFromView(viewPoint1), ModelFromView(viewPoint2));
        }

        public void PanView(int panX, int panY)
        {
            var viewCenter = ViewFromModel(ModelCenter);
            UpdateModelCenter(ModelFromView(new Point2D(
                viewCenter.X - panX,
                viewCenter.Y - panY
            )));
        }

        public object Clone()
        {
            var result = new ModelViewTransform();
            result.ModelCenter = this.ModelCenter;
            result.modelHeight = this.modelHeight;
            result.viewExtents = this.viewExtents;
            result.InitMatrices();
            return result;
        }
    }
}
